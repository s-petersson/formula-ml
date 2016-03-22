#include <iostream>
#include <core/Window.h>
#include <sim/SimulationState.h>
#include <experiments/StandardRenderer.h>
#include "NEATNetworkMidline.h"
#include "NeatTrainer.h"
#include <neural/neat/Network.h>
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>

using namespace neat;

// Static variables
int NEATNetworkMidline::nbr_of_curve_points;
float NEATNetworkMidline::termination_distance;
float NEATNetworkMidline::max_time;


//---  NEATNetworkMidline  ---//

NEATNetworkMidline::NEATNetworkMidline() {
    
}

NEATNetworkMidline::~NEATNetworkMidline() {
    delete trainer;
}

void NEATNetworkMidline::run() {
    // Decide network size
    int nbr_of_inputs = 
        5 + 
        Simulator::write_track_curve_size(nbr_of_curve_points);
    Config::set_config(nbr_of_inputs, 2);

    // Construct the NeatTrainer
    trainer = new NeatTrainer();

    trainer->evaluator_factory = []()
    {
        return new MidlineEvaluator();
    };

    // Define call backs
    trainer->on_generation_done = [](int generation)
    {
        cout << "New Generation: " << generation << endl;
    };

    trainer->on_new_best = [](neural::Network* new_best, float fitness)
    {
        MidlineEvaluator eval = MidlineEvaluator();
        SimulationResult result = eval.run(new_best);
        cout << "New maximum fitness: " << fitness << endl
            << "Distance: " << result.distance_driven << endl
            << "Time: " << result.time_alive << endl << endl;

        eval.reset(new_best);

        Window * window = new Window();
        vector<Renderer*> renderers;
        renderers.push_back(new StandardRenderer(eval.getSimulator()));
        SimulationState * s = new SimulationState(eval.getSimulator(), renderers);

        window->setState(s);
        window->run();
        delete window;
    };

    // Start the trainer
    trainer->run();
}

std::function<CarControl()> NEATNetworkMidline::updater() {
	return [&]() {
		CarControl control = CarControl();

		return control;
	};
}


//---  MidlineEvaluator  ---//

MidlineEvaluator::MidlineEvaluator() {
    simulator = new Simulator();

    // Create simulated objects
    // NOTE: Starting grid is at first "checkpoint". In order
    //       to change this, offset the checkpoint order.
    simulator->track = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));
    simulator->car = new CarModel(simulator->track->get_start_grid_pos(),
        glm::vec3(-0.616278410f, -0.787541449f, 0),
        12.f);;

    simulator->progress_timeout = 0.1f;
    simulator->termination_distance = NEATNetworkMidline::termination_distance;

    network_indata.value_count = Config::Inputs;
    network_indata.values = new float[Config::Inputs];
    network_output.value_count = Config::Outputs;
    network_output.values = new float[Config::Outputs];

    simulator->carUpdater = [&]() {
        float* inputs = network_indata.values;
        float* outputs = network_output.values;

        int i = 0;
        inputs[i++] = simulator->distance_to_middle();
        inputs[i++] = simulator->angle_to_line();
        inputs[i++] = simulator->car->getSpeed();
        int curve_data_start = i;
        simulator->write_track_curve(inputs, i, NEATNetworkMidline::nbr_of_curve_points);
        inputs[i++] = neural::sum_absolutes(&inputs[curve_data_start], simulator->write_track_curve_size(NEATNetworkMidline::nbr_of_curve_points));
        inputs[i++] = 1.0f;

        network->fire(network_indata, network_output);

        CarControl control;
        control.acceleration = 0;
        control.brake = outputs[1];
        control.steer = outputs[0];

        return control;
    };
}

MidlineEvaluator::~MidlineEvaluator() {
    delete[] network_indata.values;
    delete[] network_output.values;
    delete simulator->track;
    delete simulator->car;
}

float MidlineEvaluator::evaluate_network(neural::Network* network) {
    SimulationResult result = run(network);
    return neural::fitness_distance_time(result, NEATNetworkMidline::termination_distance, NEATNetworkMidline::max_time);
}

SimulationResult MidlineEvaluator::run(neural::Network* network) {
    reset(network);
    return simulator->run(0.01f);
}

void MidlineEvaluator::reset(neural::Network* network) {
    this->network = network;
    simulator->reset();
    simulator->car->setSpeed(12.f);
}
