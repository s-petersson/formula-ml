#include <iostream>
#include <core/Window.h>
#include <sim/SimulationState.h>
#include <experiments/StandardRenderer.h>
#include "NeatCurveDataExperiment.h"
#include "NeatTrainer.h"
#include <neural/neat/Network.h>
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>
#include <experiments/NetworkView.h>
#include <core/Keyboard.h>
using namespace neat;

// Static variables
int NeatCurveDataExperiment::nbr_of_curve_points;
float NeatCurveDataExperiment::termination_distance;
float NeatCurveDataExperiment::max_time;
float NeatCurveDataExperiment::car_speed;
float NeatCurveDataExperiment::curve_point_spacing;
float NeatCurveDataExperiment::curve_point_spacing_incremental_percentage;

//---  NeatCurveDataExperiment  ---//

NeatCurveDataExperiment::NeatCurveDataExperiment() {
	// Construct the NeatTrainer
	//trainer = new NeatTrainer();
}

NeatCurveDataExperiment::NeatCurveDataExperiment(string path) {
	// Construct the NeatTrainer
	trainer = new NeatTrainer(path);
}

NeatCurveDataExperiment::~NeatCurveDataExperiment() {
    delete trainer;
}

void NeatCurveDataExperiment::run() {
    // Decide network size
    int nbr_of_inputs = 
        5 + 
        Simulator::write_track_curve_size(nbr_of_curve_points);
   
	Config::set_config(nbr_of_inputs, 2);
	
	trainer = new NeatTrainer();
    
	trainer->evaluator_factory = []()
    {
        return new CurveEvaluator();
    };

    // Define call backs
    trainer->on_generation_done = [](int generation)
    {
        cout << "New Generation: " << generation << endl;
    };

    trainer->on_new_best = [](neural::Network* new_best, float fitness)
    {
        
		CurveEvaluator eval = CurveEvaluator();
        SimulationResult result = eval.run(new_best);
        cout << "New maximum fitness: " << fitness << endl
            << "Distance: " << result.distance_driven << endl
            << "Time: " << result.time_alive << endl << endl;
        /* No longer relevant due to the changed visualisation.
        eval.reset(new_best);

        Window * window = new Window();
        vector<Renderer*> renderers;
        renderers.push_back(new StandardRenderer(eval.getSimulator()));
        SimulationState * s = new SimulationState(eval.getSimulator(), renderers);

        window->setState(s);
        window->run();
        delete window;
		*/
    };

    // Start the trainer
	std::thread tt = std::thread(&NeatTrainer::run, trainer);
    visualise();
	tt.join();
}

void NeatCurveDataExperiment::visualise() {

	Simulator* simulator = new Simulator();
	Network * network = new Network();
	// Create simulated objects
	// NOTE: Starting grid is at first "checkpoint". In order
	//       to change this, offset the checkpoint order.
	simulator->track = new TrackModel(glm::vec3());
	simulator->car = new CarModel(simulator->track->get_start_grid_pos(), glm::vec3(0, 1, 0), NeatCurveDataExperiment::car_speed);

	simulator->progress_timeout = 5.0f;
	simulator->termination_distance = NeatCurveDataExperiment::termination_distance;
	neural::NetworkIO network_input, network_output;
	network_input.value_count = Config::Inputs;
	network_input.values = new float[Config::Inputs];
	network_output.value_count = Config::Outputs;
	network_output.values = new float[Config::Outputs];

	StandardRenderer * sr;
	NetworkView * nv;
	SimulationState * s;

	simulator->carUpdater = [&]() {
		if (simulator->has_terminated() || isKeyDown(GLFW_KEY_HOME)) {
			simulator->reset();
			delete network;
			network = new Network(trainer->get_best().genes);
			delete nv;
			nv = new NetworkView(network);
			s->clear_renderers();
			s->add_renderer(sr);
			s->add_renderer(nv);
		}
		float* inputs = network_input.values;
		float* outputs = network_output.values;

		int i = 0;
		inputs[i++] = simulator->distance_to_middle();
		inputs[i++] = simulator->angle_to_line();
		inputs[i++] = simulator->car->getSpeed();
		int curve_data_start = i;
		simulator->write_track_curve(inputs, i, NeatCurveDataExperiment::nbr_of_curve_points, curve_point_spacing_incremental_percentage, curve_point_spacing_incremental_percentage);
		inputs[i++] = neural::sum_absolutes(&inputs[curve_data_start], 
											simulator->write_track_curve_size(NeatCurveDataExperiment::nbr_of_curve_points));
		inputs[i++] = 1.0f;

        neural::flip_parity_if(&inputs[curve_data_start], NeatCurveDataExperiment::nbr_of_curve_points, inputs[curve_data_start] > 0);

		network->fire(network_input, network_output);

		CarControl control;
		control.acceleration = 0;
		control.brake = outputs[1];
		control.steer = outputs[0];
        control.steer = inputs[curve_data_start] > 0 ? control.steer : -control.steer;

		return control;
	};

	Window * window = new Window();
	vector<Renderer*> renderers;

	sr = new StandardRenderer(simulator);
	nv = new NetworkView(network);
	renderers.push_back(sr);
	s = new SimulationState(simulator, renderers);

	window->setState(s);
	window->run();
	delete window;
	//delete s;
	//delete sr;
}



//---  CurveEvaluator  ---//

CurveEvaluator::CurveEvaluator() {
    simulator = new Simulator();

    // Create simulated objects
    // NOTE: Starting grid is at first "checkpoint". In order
    //       to change this, offset the checkpoint order.
    simulator->track = new TrackModel(glm::vec3());
    simulator->car = new CarModel(simulator->track->get_start_grid_pos(),glm::vec3(0,1,0), NeatCurveDataExperiment::car_speed);

    simulator->progress_timeout = 5.0f;
    simulator->termination_distance = NeatCurveDataExperiment::termination_distance;

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
        simulator->write_track_curve(inputs, i, 
            NeatCurveDataExperiment::nbr_of_curve_points, 
            NeatCurveDataExperiment::curve_point_spacing,
            NeatCurveDataExperiment::curve_point_spacing_incremental_percentage);
        inputs[i++] = neural::sum_absolutes(&inputs[curve_data_start],
											simulator->write_track_curve_size(NeatCurveDataExperiment::nbr_of_curve_points));
        inputs[i++] = 1.0f;
        neural::flip_parity_if(&inputs[curve_data_start], NeatCurveDataExperiment::nbr_of_curve_points, inputs[curve_data_start] > 0);

        network->fire(network_indata, network_output);

        CarControl control;
        control.acceleration = 0;
        control.brake = outputs[1];
        control.steer = outputs[0];
        control.steer = inputs[curve_data_start] > 0 ? control.steer : -control.steer;

        return control;
    };
}

CurveEvaluator::~CurveEvaluator() {
    delete[] network_indata.values;
    delete[] network_output.values;
    delete simulator->track;
    delete simulator->car;
}

float CurveEvaluator::evaluate_network(neural::Network* network) {
    SimulationResult result = run(network);
    return neural::fitness_distance_time(result, NeatCurveDataExperiment::termination_distance, NeatCurveDataExperiment::max_time);
}

SimulationResult CurveEvaluator::run(neural::Network* network) {
    reset(network);
    return simulator->run(0.01f);
}

void CurveEvaluator::reset(neural::Network* network) {
    this->network = network;
    simulator->reset();
    simulator->car->setSpeed(12.f);
}
