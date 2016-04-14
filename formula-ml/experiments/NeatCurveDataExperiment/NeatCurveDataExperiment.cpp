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
#include <experiments/SimulationEvaluator.h>
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

NeatCurveDataExperiment::NeatCurveDataExperiment() { }

NeatCurveDataExperiment::NeatCurveDataExperiment(string path_to_network) {
    this->load_network_path = path_to_network;
}

NeatCurveDataExperiment::~NeatCurveDataExperiment() {
    delete trainer;
    if (window) delete window;
}

void NeatCurveDataExperiment::run() {
    // Decide network size
    int nbr_of_inputs = 5 + Simulator::write_track_curve_size(nbr_of_curve_points);

	Config::set_config(nbr_of_inputs, 2);
    Config::sigmoid = [](float x) {
        return -1.0f + 2.0f / (1.0f + glm::exp(-x));
    };

    // Prepare the experiment
    SimulationEvaluator experiment = SimulationEvaluator();
    experiment.termination_distance = termination_distance;
    experiment.max_time = max_time;
    experiment.car_speed = car_speed;

    experiment.nbr_of_curve_points = nbr_of_curve_points;
    experiment.curve_point_spacing = curve_point_spacing;
    experiment.curve_point_spacing_incremental_percentage = curve_point_spacing_incremental_percentage;

    experiment.init();

    std::function<SimulationEvaluator*()> factory = experiment.makeFactory();

    // Prepare the window
    SimulationEvaluator* windowEnvironment = factory();
    window = new ExperimentWindow(windowEnvironment->simulator);
    window->setNetworkLocation(&windowEnvironment->network, true);

    // Create the NeatTrainer
    if(this->load_network_path != "") {
        trainer = new NeatTrainer(this->load_network_path);
    } else {
        trainer = new NeatTrainer();
    }

    trainer->evaluator_factory = factory;

    // Define call backs
    trainer->on_generation_done = [](int generation)
    {
        cout << "New Generation: " << generation << endl;
    };

    SimulationEvaluator* resultFetcher = factory();
    trainer->on_new_best = [&, resultFetcher](neat::Genome* new_best, float fitness)
    {
        Network *n = new Network(new_best->genes);
        SimulationResult result = resultFetcher->run(n);

        cout << "New maximum fitness: " << fitness                  << endl
             << "Distance: "            << result.distance_driven   << endl
             << "Time: "                << result.time_alive        << endl << endl;

        window->updateNetwork(new Network(new_best->genes));
        delete n;
    };

    // Start the trainer
	std::thread tt = std::thread(&NeatTrainer::run, trainer);
    window->run();
	tt.join();
}
