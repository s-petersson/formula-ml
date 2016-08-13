#include "AlternatingTrackExperiment.h"
#include <iostream>
#include <thread>
#include <neural/Helpers.h>
#include <neural/neat/Constants.h>

#ifndef CLOUD_COMPUTING
#include <core/Window.h>
#include <experiments/NetworkView.h>
#include <experiments/ExperimentWindow.h>
#endif

using namespace neat;
using namespace std;

AlternatingTrackExperiment::AlternatingTrackExperiment() { }

AlternatingTrackExperiment::AlternatingTrackExperiment(string path_to_network) {
    this->load_network_path = path_to_network;
}

AlternatingTrackExperiment::~AlternatingTrackExperiment() {
}

void AlternatingTrackExperiment::run() {
    neural::FileWriter::clearFile("./log/AlternatingTrackExperiment.txt");
    Config::sigmoid = [](float x) {
        return -1.0f + 2.0f / (1.0f + glm::exp(-x));
    };

    // Prepare the experiment
    AiSettings ai_settings;
    ai_settings.angle_to_line = true;
    ai_settings.distance_to_middle = true;
    ai_settings.distance_to_edges = true;
    ai_settings.speed = true;
    ai_settings.curve_data = true;
    ai_settings.curve_data_segment_sums = true;
    ai_settings.curve_data_sum_absolutes = false;

    ai_settings.nbr_of_curve_points = 10;
    ai_settings.curve_point_spacing = 10.f;
    ai_settings.curve_point_spacing_incremental_percentage = 0.35f;

    ai_settings.output_speed = true;

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
    sim_settings.track_path = new string("./res/models/circuit_normal.model");
    sim_settings.completeTrack = true;
    sim_settings.termination_distance = 5200.f;
    sim_settings.max_time = 1500.f;

    sim_settings.car_speed_max = 97.f;
    sim_settings.car_speed_initial = 0.f;
    sim_settings.min_avg_speed = 3.f;
    sim_settings.avg_speed_excemption_distance = 140.f;

    windowEnvironment->sim_settings = sim_settings;
    windowEnvironment->init();

#ifndef CLOUD_COMPUTING
    auto _eval = new SimulationEvaluator();
    _eval->ai_settings = ai_settings;
	shared_ptr<RacelineLogger> raceline_logger = make_shared<RacelineLogger>(_eval);
    window = make_shared<ExperimentWindow>(windowEnvironment->getSimulator(), trainer, raceline_logger);
    window->setNetworkLocation(windowEnvironment->getNetworkLocation(), true);
#endif

    // Define call backs
    trainer->evaluator_factory = [=]() {
        return new _Evaluator(ai_settings);
    };

    trainer->on_generation_done = [](int generation)
    {
        string output = "New Generation: " + to_string(generation);
        output += ", Active Evaluator: " + to_string(_Evaluator::current_evaluator);
        output += "\n";

        int nbr_of_evaluators = 6;
        int generations_per_evaluator = 10;

        _Evaluator::current_evaluator_count++;
        if (_Evaluator::current_evaluator_count >= generations_per_evaluator) {
            _Evaluator::current_evaluator_count = 0;
            _Evaluator::current_evaluator++;
            _Evaluator::current_evaluator %= nbr_of_evaluators;
            output += "Evaluator changed: " + to_string(_Evaluator::current_evaluator) + "\n";
        }

#ifndef CLOUD_COMPUTING
        cout << output;
#else
        neural::FileWriter::stringToFile("./log/NeatCurveDataExperiment.txt", output);
#endif
    };

    trainer->on_new_best = [&](EvaluationResult evaluationResult, Genome genome)
    {
        _Evaluator::print(evaluationResult);
    };

    // Start the trainer
    std::thread tt = std::thread(&Trainer::run, trainer);
#ifndef CLOUD_COMPUTING
    window->run();
#endif
    tt.join();
}

// Static variables
int AlternatingTrackExperiment::_Evaluator::current_evaluator = 0;
int AlternatingTrackExperiment::_Evaluator::current_evaluator_count = 0;

AlternatingTrackExperiment::_Evaluator::_Evaluator(AiSettings ai_settings) {

    {
        SimulationEvaluator* track_evaluator = new SimulationEvaluator();

        SimulatorSettings sim_settings = SimulatorSettings();
        sim_settings.track_path = new string("./res/models/corners/corner_30_r.model");
        sim_settings.completeTrack = false;
        sim_settings.termination_distance = 820.f;
        sim_settings.max_time = 150.f;

        sim_settings.car_speed_max = 97.f;
        sim_settings.car_speed_initial = 0.f;
        sim_settings.min_avg_speed = 3.f;
        sim_settings.avg_speed_excemption_distance = 140.f;

        track_evaluator->sim_settings = sim_settings;
        track_evaluator->ai_settings = ai_settings;
        track_evaluator->init();

        evaluators.push_back(track_evaluator);
    }
    {
        SimulationEvaluator* track_evaluator = new SimulationEvaluator();

        SimulatorSettings sim_settings = SimulatorSettings();
        sim_settings.track_path = new string("./res/models/corners/corner_30_l.model");
        sim_settings.completeTrack = false;
        sim_settings.termination_distance = 820.f;
        sim_settings.max_time = 150.f;

        sim_settings.car_speed_max = 97.f;
        sim_settings.car_speed_initial = 0.f;
        sim_settings.min_avg_speed = 3.f;
        sim_settings.avg_speed_excemption_distance = 140.f;

        track_evaluator->sim_settings = sim_settings;
        track_evaluator->ai_settings = ai_settings;
        track_evaluator->init();

        evaluators.push_back(track_evaluator);
    }
    {
        SimulationEvaluator* track_evaluator = new SimulationEvaluator();

        SimulatorSettings sim_settings = SimulatorSettings();
        sim_settings.track_path = new string("./res/models/corners/corner_90_10_r.model");
        sim_settings.completeTrack = false;
        sim_settings.termination_distance = 925.f;
        sim_settings.max_time = 400.f;

        sim_settings.car_speed_max = 97.f;
        sim_settings.car_speed_initial = 0.f;
        sim_settings.min_avg_speed = 3.f;
        sim_settings.avg_speed_excemption_distance = 140.f;

        track_evaluator->sim_settings = sim_settings;
        track_evaluator->ai_settings = ai_settings;
        track_evaluator->init();

        evaluators.push_back(track_evaluator);
    }
    {
        SimulationEvaluator* track_evaluator = new SimulationEvaluator();

        SimulatorSettings sim_settings = SimulatorSettings();
        sim_settings.track_path = new string("./res/models/corners/corner_90_10_l.model");
        sim_settings.completeTrack = false;
        sim_settings.termination_distance = 925.f;
        sim_settings.max_time = 400.f;

        sim_settings.car_speed_max = 97.f;
        sim_settings.car_speed_initial = 0.f;
        sim_settings.min_avg_speed = 3.f;
        sim_settings.avg_speed_excemption_distance = 140.f;

        track_evaluator->sim_settings = sim_settings;
        track_evaluator->ai_settings = ai_settings;
        track_evaluator->init();

        evaluators.push_back(track_evaluator);
    }
    {
        SimulationEvaluator* track_evaluator = new SimulationEvaluator();

        SimulatorSettings sim_settings = SimulatorSettings();
        sim_settings.track_path = new string("./res/models/corners/corner_180_r_new.model");
        sim_settings.completeTrack = false;
        sim_settings.termination_distance = 975.f;
        sim_settings.max_time = 400.f;

        sim_settings.car_speed_max = 97.f;
        sim_settings.car_speed_initial = 0.f;
        sim_settings.min_avg_speed = 3.f;
        sim_settings.avg_speed_excemption_distance = 140.f;

        track_evaluator->sim_settings = sim_settings;
        track_evaluator->ai_settings = ai_settings;
        track_evaluator->init();

        evaluators.push_back(track_evaluator);
    }
    {
        SimulationEvaluator* track_evaluator = new SimulationEvaluator();

        SimulatorSettings sim_settings = SimulatorSettings();
        sim_settings.track_path = new string("./res/models/corners/corner_180_l_new.model");
        sim_settings.completeTrack = false;
        sim_settings.termination_distance = 975.f;
        sim_settings.max_time = 400.f;

        sim_settings.car_speed_max = 97.f;
        sim_settings.car_speed_initial = 0.f;
        sim_settings.min_avg_speed = 3.f;
        sim_settings.avg_speed_excemption_distance = 140.f;

        track_evaluator->sim_settings = sim_settings;
        track_evaluator->ai_settings = ai_settings;
        track_evaluator->init();

        evaluators.push_back(track_evaluator);
    }
}

AlternatingTrackExperiment::_Evaluator::~_Evaluator() {
    evaluators.erase(evaluators.begin(), evaluators.end());
}

EvaluationResult AlternatingTrackExperiment::_Evaluator::evaluate_network(neat::Network* network) {
    EvaluationResult result = EvaluationResult();

    result = evaluators[current_evaluator]->evaluate_network(network);
    result.fitness = neural::fitness_distance_time_exp(result.simResult, evaluators[current_evaluator]->sim_settings.termination_distance, 300, 20);

    /*
    for (int i = 0; i < evaluators.size(); i++) {
        EvaluationResult er = evaluators[i]->evaluate_network(network);
        er.fitness = neural::fitness_distance_time_exp(er.simResult, evaluators[i]->sim_settings.termination_distance, 300, 20);
        result.partialResults.push_back(er);
    }
    result.fitness = result.partialResults[current_evaluator].fitness;
    result.simResult.distance_on_track = result.partialResults[current_evaluator].simResult.distance_on_track;
    result.simResult.distance_driven = result.partialResults[current_evaluator].simResult.distance_driven;
    */

    return result;
}

void AlternatingTrackExperiment::_Evaluator::print(const EvaluationResult& result) {
    string output;

    output += "\nNew Best, evaluator " + to_string(current_evaluator) + "\n";
    output += "Fitness:  " + to_string(result.fitness) + "\n";
    output += "Fistance: " + to_string(result.simResult.distance_on_track) + "\n";
    output += "Time:     " + to_string(result.simResult.time_alive) + "\n";

    /*
    trackStatus = (current_evaluator == 0 ? " ACTIVE" : "");
    output += "Evaluator 0, 30 right: " + trackStatus + "\n";
    output += " Fitness:  " + to_string(result.partialResults[2].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[2].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[2].simResult.time_alive) + "\n";

    trackStatus = (current_evaluator == 1 ? " ACTIVE" : "");
    output += "Evaluator 1, 30 left: " + trackStatus + "\n";
    output += " Fitness:  " + to_string(result.partialResults[3].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[3].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[3].simResult.time_alive) + "\n";

    trackStatus = (current_evaluator == 2 ? " ACTIVE" : "");
    output += "Evaluator 2, 90 right: " + trackStatus + "\n";
    output += " Fitness:  " + to_string(result.partialResults[4].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[4].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[4].simResult.time_alive) + "\n";

    trackStatus = (current_evaluator == 3 ? " ACTIVE" : "");
    output += "Evaluator 3, 90 left: " + trackStatus + "\n";
    output += " Fitness:  " + to_string(result.partialResults[5].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[5].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[5].simResult.time_alive) + "\n";

    trackStatus = (current_evaluator == 4 ? " ACTIVE" : "");
    output += "Evaluator 4, 180 right: " + trackStatus + "\n";
    output += " Fitness:  " + to_string(result.partialResults[6].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[6].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[6].simResult.time_alive) + "\n";

    trackStatus = (current_evaluator == 5 ? " ACTIVE" : "");
    output += "Evaluator 4, 180 left: " + trackStatus + "\n";
    output += " Fitness:  " + to_string(result.partialResults[6].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[6].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[6].simResult.time_alive) + "\n";
    */

#ifndef CLOUD_COMPUTING
    cout << output;
#else
    neural::FileWriter::stringToFile("./log/NeatCurveDataExperiment.txt", output);
#endif
}

void AlternatingTrackExperiment::_Evaluator::reset() {
    for (auto& sim_evaluator : evaluators) {
        sim_evaluator->reset();
    }
}