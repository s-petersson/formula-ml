#include <iostream>
#include <core/Window.h>
#include <sim/SimulationState.h>
#include <experiments/StandardRenderer.h>
#include "NeatCurveDataExperiment.h"
#include <neural/neat/Trainer.h>
#include <neural/neat/Network.h>
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>
#include <experiments/NetworkView.h>
#include <experiments/SimulationEvaluator.h>
#include <core/Keyboard.h>
#include <thread>
using namespace neat;


NeatCurveDataExperiment::NeatCurveDataExperiment() { }

NeatCurveDataExperiment::NeatCurveDataExperiment(string path_to_network) {
    this->load_network_path = path_to_network;
}

NeatCurveDataExperiment::~NeatCurveDataExperiment() {
    //delete trainer;
    //if (window) delete window;
}

void NeatCurveDataExperiment::run() {
    Config::sigmoid = [](float x) {
        return -1.0f + 2.0f / (1.0f + glm::exp(-x));
    };

    // Prepare the experiment
    print_settings(ai_settings);
    set_neat_config(ai_settings);

    SimulationEvaluator experiment = SimulationEvaluator();
    experiment.sim_settings = sim_settings;
    experiment.ai_settings = ai_settings;
    experiment.init();

    std::function<SimulationEvaluator*()> factory = experiment.makeFactory();

    // Create the Trainer
    if (this->load_network_path != "") {
        trainer = make_shared<Trainer>(this->load_network_path);
    }
    else {
        trainer = make_shared<Trainer>();
    }

    // Prepare the window
    SimulationEvaluator* windowEnvironment = factory();
    window = make_shared<ExperimentWindow>(windowEnvironment->getSimulator(), trainer);
    window->setNetworkLocation(windowEnvironment->getNetworkLocation(), true);
	

    trainer->evaluator_factory = factory;

    // Define call backs
    trainer->on_generation_done = [](int generation)
    {
        cout << "New Generation: " << generation << endl;
    };

    trainer->on_new_best = [&](EvaluationResult evaluationResult)
    {
        cout << "New maximum fitness: " << evaluationResult.fitness                     << endl
             << "Distance on track:   " << evaluationResult.simResult.distance_on_track << endl
             << "Time: "                << evaluationResult.simResult.time_alive        << endl << endl;

    };
	
    // Start the trainer
	std::thread tt = std::thread(&Trainer::run, trainer);
    window->run();
	tt.join();
}
