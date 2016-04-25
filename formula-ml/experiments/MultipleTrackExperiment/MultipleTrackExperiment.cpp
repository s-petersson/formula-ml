#include <iostream>
#include <thread>
#include <core/Window.h>
#include "MultipleTrackExperiment.h"

#include <neural/neat/Trainer.h>
#include <neural/neat/Network.h>
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>

#include <experiments/NetworkView.h>
#include <experiments/SimulationEvaluator.h>
#include <experiments/ExperimentWindow.h>
using namespace neat;

MultipleTrackExperiment::MultipleTrackExperiment() { }

MultipleTrackExperiment::MultipleTrackExperiment(string path_to_network) {
    this->load_network_path = path_to_network;
}

MultipleTrackExperiment::~MultipleTrackExperiment() {
}

void MultipleTrackExperiment::run() {
    Config::sigmoid = [](float x) {
        return -1.0f + 2.0f / (1.0f + glm::exp(-x));
    };

    // Prepare the experiment
    AiSettings ai_settings;
    ai_settings.angle_to_line = true;
    ai_settings.distance_to_middle = true;
    ai_settings.speed = true;
    ai_settings.curve_data = true;
    ai_settings.curve_data_sum_absolutes = true;

    ai_settings.nbr_of_curve_points = 5;
    ai_settings.curve_point_spacing = 15.f;
    ai_settings.curve_point_spacing_incremental_percentage = 0.3f;

    print_settings(ai_settings);
    set_neat_config(ai_settings);

    // Create the Trainer
    if (this->load_network_path != "") {
        trainer = make_shared<Trainer>(this->load_network_path);
    }
    else {
        trainer = make_shared<Trainer>();
    }

    // Prepare the window
    SimulationEvaluator* windowEnvironment = new SimulationEvaluator();
    windowEnvironment->ai_settings = ai_settings;

    SimulatorSettings sim_settings = SimulatorSettings();
    sim_settings.track_path = new string("./res/models/circuit_wide.model");
    sim_settings.termination_distance = 5200.f;
    sim_settings.max_time = 400.f;
    sim_settings.car_speed = 15.f;

    windowEnvironment->sim_settings = sim_settings;
    windowEnvironment->init();

    window = make_shared<ExperimentWindow>(windowEnvironment->getSimulator(), trainer);
    window->setNetworkLocation(windowEnvironment->getNetworkLocation(), true);

    // Define call backs
    trainer->evaluator_factory = [=]() {
        return new _Evaluator(ai_settings);
    };

    trainer->on_generation_done = [](int generation)
    {
        cout << "New Generation: " << generation << endl;
    };

    trainer->on_new_best = [&](EvaluationResult evaluationResult)
    {
        _Evaluator::print(evaluationResult);
    };

    // Start the trainer
    std::thread tt = std::thread(&Trainer::run, trainer);
    window->run();
    tt.join();
}



MultipleTrackExperiment::_Evaluator::_Evaluator(AiSettings ai_settings){

    {
        SimulationEvaluator* track_evaluator = new SimulationEvaluator();

        SimulatorSettings sim_settings = SimulatorSettings();
        sim_settings.track_path = new string("./res/models/circuit_narrow.model");
        sim_settings.termination_distance = 5200.f;
        sim_settings.max_time = 400.f;
        sim_settings.car_speed = 15.f;

        track_evaluator->sim_settings = sim_settings;
        track_evaluator->ai_settings = ai_settings;
        track_evaluator->init();

        evaluators.push_back(track_evaluator);
    }
    {
        SimulationEvaluator* track_evaluator = new SimulationEvaluator();

        SimulatorSettings sim_settings = SimulatorSettings();
        sim_settings.track_path = new string("./res/models/circuit_wide.model");
        sim_settings.termination_distance = 5200.f;
        sim_settings.max_time = 400.f;
        sim_settings.car_speed = 15.f;

        track_evaluator->sim_settings = sim_settings;
        track_evaluator->ai_settings = ai_settings;
        track_evaluator->init();

        evaluators.push_back(track_evaluator);
    }
}

MultipleTrackExperiment::_Evaluator::~_Evaluator(){ 
    evaluators.erase(evaluators.begin(), evaluators.end());
}

EvaluationResult MultipleTrackExperiment::_Evaluator::evaluate_network(neat::Network* network) {
    EvaluationResult result = EvaluationResult();

    EvaluationResult ev0 = evaluators[0]->evaluate_network(network);
    ev0.fitness = neural::fitness_distance_time_exp(ev0.simResult, evaluators[0]->sim_settings.termination_distance, 500, 100);
    result.partialResults.push_back(ev0);

    EvaluationResult ev1 = evaluators[1]->evaluate_network(network);
    ev1.fitness = neural::fitness_distance_time_exp(ev1.simResult, evaluators[1]->sim_settings.termination_distance, 500, 100);
    result.partialResults.push_back(ev1);

    result.fitness = ev0.fitness + ev1.fitness;
    result.simResult.distance_driven = ev0.simResult.distance_driven + ev1.simResult.distance_driven;
    result.simResult.time_alive = ev0.simResult.time_alive + ev1.simResult.time_alive;

    return result;
}

void MultipleTrackExperiment::_Evaluator::print(const EvaluationResult& result) {
    cout << endl 
         << "New Best:"                                                 << endl
         << "Total fitness:  "  << result.fitness                       << endl
         << "Total distance: "  << result.simResult.distance_driven     << endl
         << "Total time:     "  << result.simResult.time_alive          << endl;

    cout << "Evaluator 0, narrow track:"                                          << endl
         << " Fitness:  "   << result.partialResults[0].fitness                   << endl
         << " Distance: "   << result.partialResults[0].simResult.distance_driven << endl
         << " Time:     "   << result.partialResults[0].simResult.time_alive      << endl;
    
    cout << "Evaluator 1, wide track:"                                            << endl
         << " Fitness:  "   << result.partialResults[1].fitness                   << endl
         << " Distance: "   << result.partialResults[1].simResult.distance_driven << endl
         << " Time:     "   << result.partialResults[1].simResult.time_alive      << endl;

    cout << endl;
}

void MultipleTrackExperiment::_Evaluator::reset() {
    for (auto& sim_evaluator : evaluators) {
        sim_evaluator->reset();
    }
}