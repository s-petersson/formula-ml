#define CLOUD_COMPUTING

#include "NeatCurveDataExperiment.h"
#include <thread>
#include <iostream>

#ifndef CLOUD_COMPUTING
#include <sstream>

#include <core/Window.h>
#include <core/Keyboard.h>

#include <sim/SimulationState.h>

#include <experiments/RacelineLogger.h>
#include <experiments/StandardRenderer.h>
#include <experiments/NetworkView.h>
#endif

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

    SimulationEvaluator *eval = new SimulationEvaluator();
    eval->sim_settings = sim_settings;
    eval->ai_settings = ai_settings;
    eval->init();


#ifndef CLOUD_COMPUTING
    // Prepare the window
	shared_ptr<RacelineLogger> raceline_logger = make_shared<RacelineLogger>(eval);
    SimulationEvaluator* windowEnvironment = factory();
    window = make_shared<ExperimentWindow>(windowEnvironment->getSimulator(), trainer, raceline_logger);
    window->setNetworkLocation(windowEnvironment->getNetworkLocation(), true);
    raceline_logger->init();
#endif

    trainer->evaluator_factory = factory;
    // Define callbacks
    trainer->on_generation_done = [&](int generation)
    {
        cout << "New Generation: " << generation << endl;

#ifndef CLOUD_COMPUTING
        stringstream ss;
		ss << trainer->savePath <<"generation_"<< generation << ".png";
		
        RacelineLoggerJob job;
        job.genome = trainer->get_best();
        job.location = ss.str();
		
		raceline_logger->add_job(job);
#endif
    };

    trainer->on_new_best = [&](EvaluationResult evaluationResult)
    {
        cout << "New maximum fitness: " << evaluationResult.fitness                     << endl
             << "Distance on track:   " << evaluationResult.simResult.distance_on_track << endl
             << "Distance driven:     " << evaluationResult.simResult.distance_driven   << endl
             << "Time: "                << evaluationResult.simResult.time_alive        << endl << endl;

    };
	

    // Start the trainer
	std::thread tt = std::thread(&Trainer::run, trainer);
#ifndef CLOUD_COMPUTING
    window->run();
#endif
	tt.join();
}
