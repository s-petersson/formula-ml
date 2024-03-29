#include "SimulationEvaluator.h"
#include <neural/neat/Trainer.h>
#include <neural/neat/Network.h>
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>
#include <iostream>

using namespace neat;


int required_nbr_of_inputs(const AiSettings& settings) {
    int sum = 0;

    if (settings.distance_to_middle) sum++;
    if (settings.distance_to_edges)  sum=sum+2;
    if (settings.angle_to_line)      sum++;
    if (settings.speed)              sum++;

    if (settings.curve_data) {
        // TODO: Wat? write track curve returns nbr of curve points
        sum += Simulator::write_track_curve_size(settings.nbr_of_curve_points);
		if (settings.curve_data_segment_sums) sum += 3; // Hard coded, fix if Helpers::write_segment_sums support other
        if (settings.curve_data_sum_absolutes) sum++;
    }

    if (settings.checkpoint_data) {
        sum += Simulator::write_checkpoints_size(settings.checkpoint_data_nbr);
    }

    return sum;
}

int required_nbr_of_outputs(const AiSettings& settings) {
    return 1 + (settings.output_speed ? 1 : 0);
}

void set_neat_config(const AiSettings& settings) {
    Config::set_config(
        required_nbr_of_inputs(settings),
        required_nbr_of_outputs(settings)
    );
}

void print_settings_helper(int& counter, bool predicate, const std::string& description) {
    if (predicate) {
        cout << endl << counter++ << ". " << description;
    }
}
void print_settings(const AiSettings& settings) {
    cout << endl << "Configuration of ai input values:";

    int i = 0;
    print_settings_helper(i, true, "Bias");
    print_settings_helper(i, settings.distance_to_middle, "Distance to middle");
    print_settings_helper(i, settings.distance_to_edges, "Distance to left edge");
    print_settings_helper(i, settings.distance_to_edges, "Distance to right edge");
    print_settings_helper(i, settings.angle_to_line, "Angle to mid line");
    print_settings_helper(i, settings.speed, "Car speed");

    if (settings.curve_data) {
        float distance = settings.curve_point_spacing;
        float accumulated_distance = 0.f;
        for (int p = 0; p < settings.nbr_of_curve_points; p++) {
            print_settings_helper(i, true, "Curve data ");
            accumulated_distance += distance;
            cout << accumulated_distance << " meters";
            distance *= 1.f + settings.curve_point_spacing_incremental_percentage;
        }
		print_settings_helper(i, settings.curve_data_segment_sums, "Curve segment, point 1-4");
		print_settings_helper(i, settings.curve_data_segment_sums, "Curve segment, point 4-7");
		print_settings_helper(i, settings.curve_data_segment_sums, "Curve segment, point 7-10");
		print_settings_helper(i, settings.curve_data_sum_absolutes, "Sum of absolute angles (Curve data)");
	}
    if (settings.checkpoint_data) {
        print_settings_helper(i, true, "Checkpoint data");
        i += Simulator::write_track_curve_size(settings.checkpoint_data_nbr) - 1;
    }

    cout << endl << endl;

    cout << "Configuration of ai output values: " << endl;
    int j = 0;
    print_settings_helper(j, true, "Steering");
    print_settings_helper(j, settings.output_speed, "Acceleration/Braking");

    cout << endl << endl;
}

SimulationEvaluator::SimulationEvaluator() {
    network = nullptr;
}

SimulationEvaluator::~SimulationEvaluator() {
    if (network_indata.values) delete[] network_indata.values;
    if (network_output.values) delete[] network_output.values;
    if (simulator) delete simulator;
}

void SimulationEvaluator::appendIf(bool predicate, float value) {
    if (predicate) {
        network_indata.values[input_iterator++] = value;
    }
}

void SimulationEvaluator::init() {
    simulator = new Simulator(sim_settings.min_avg_speed, sim_settings.avg_speed_excemption_distance);

    // Create simulated objects
    // NOTE: Starting grid is at first "checkpoint". In order
    //       to change this, offset the checkpoint order.

    simulator->track = new TrackModel(glm::vec3(), *sim_settings.track_path, sim_settings.completeTrack);
    simulator->car = new CarModel(simulator->track->get_start_grid_pos(), glm::vec3(0, 1, 0), sim_settings.car_speed_initial, sim_settings.car_speed_min, sim_settings.car_speed_max);

    simulator->progress_timeout = 1.0f;
    simulator->termination_time = sim_settings.max_time;
    simulator->termination_distance = sim_settings.termination_distance;

    if (!network) {
        network = new Network();
    }

    network_indata.value_count = Config::Inputs;
    network_indata.values = new float[Config::Inputs];
    network_output.value_count = Config::Outputs;
    network_output.values = new float[Config::Outputs];

    simulator->carUpdater = [&]() {

        this->input_iterator = 0;

        float* inputs = network_indata.values;
        float* outputs = network_output.values;

        appendIf(ai_settings.distance_to_middle, simulator->distance_to_middle());

        int distance_to_edges_start = input_iterator;
        appendIf(ai_settings.distance_to_edges,  simulator->distance_to_left_edge());
        appendIf(ai_settings.distance_to_edges,  simulator->distance_to_right_edge());

        appendIf(ai_settings.angle_to_line,      simulator->angle_to_line());
        appendIf(ai_settings.speed,              simulator->car->getSpeed());

        int curve_data_start = input_iterator;
        if (ai_settings.curve_data) {
            simulator->write_track_curve(inputs,
                                         input_iterator,
                                         ai_settings.nbr_of_curve_points,
                                         ai_settings.curve_point_spacing,
                                         ai_settings.curve_point_spacing_incremental_percentage);

			if (ai_settings.curve_data_segment_sums) {
				neural::write_segment_sums(&inputs[curve_data_start], ai_settings.nbr_of_curve_points, &inputs[input_iterator], 3);
				input_iterator += 3;
			}

            appendIf(ai_settings.curve_data_sum_absolutes,
                     neural::sum_absolutes(&inputs[curve_data_start],
                                           Simulator::write_track_curve_size(ai_settings.nbr_of_curve_points)));
            
            if (ai_settings.curve_data_flip) {
                neural::flip_parity_if(&inputs[curve_data_start], 
                                        ai_settings.nbr_of_curve_points, 
                                        inputs[curve_data_start] > 0);
                if (ai_settings.distance_to_edges && inputs[curve_data_start] > 0) {
                    int i = distance_to_edges_start;
                    float temp = inputs[i];
                    inputs[i] = inputs[i + 1];
                    inputs[i + 1] = temp;
                }
            }
        }
        
        if (ai_settings.checkpoint_data) {
            simulator->write_checkpoints(inputs, input_iterator, ai_settings.nbr_of_curve_points);
        }        

        network->fire(network_indata, network_output);

        CarControl control = CarControl();

        control.steer = outputs[0];
        if (ai_settings.curve_data && ai_settings.curve_data_flip) {
            control.steer = inputs[curve_data_start] > 0 ? control.steer : -control.steer;
        }

        if (ai_settings.output_speed) {
            control.acceleration = outputs[1];
        }

        return control;
    };
}

EvaluationResult SimulationEvaluator::evaluate_network(neat::Network* network) {
    this->network = network;
    reset();

    EvaluationResult result = EvaluationResult();
    result.simResult = simulator->run(0.01f);
    result.fitness = neural::fitness_distance_time(result.simResult,
                                                   sim_settings.termination_distance,
                                                   sim_settings.max_time);
    return result;
}

void SimulationEvaluator::reset() {
    network->reset();
    simulator->reset();
}

std::function<SimulationEvaluator*()> SimulationEvaluator::makeFactory() {
    return [this]() {
        SimulationEvaluator* copy = new SimulationEvaluator(*this);
		//SimulationEvaluator* copy = new SimulationEvaluator();
        copy->init();
        return copy;
    };
}

Simulator* SimulationEvaluator::getSimulator() {
    return simulator;
}

void SimulationEvaluator::setNetwork(neat::Network * network) {
    this->network = network;
}

neat::Network** SimulationEvaluator::getNetworkLocation() {
    return &network;
}