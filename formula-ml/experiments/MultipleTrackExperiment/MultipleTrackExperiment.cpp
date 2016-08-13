#include "MultipleTrackExperiment.h"
#include <iostream>
#include <thread>
#include <neural/Helpers.h>
#include <neural/neat/Constants.h>

#ifndef CLOUD_COMPUTING
#include <core/Window.h>
#include <experiments/NetworkView.h>
#include <experiments/SimulationEvaluator.h>
#include <experiments/ExperimentWindow.h>
#endif

using namespace neat;
using namespace std;

MultipleTrackExperiment::MultipleTrackExperiment() { }

MultipleTrackExperiment::MultipleTrackExperiment(string path_to_network) {
    this->load_network_path = path_to_network;
}

MultipleTrackExperiment::~MultipleTrackExperiment() {
}

void MultipleTrackExperiment::run() {
    neural::FileWriter::clearFile("./log/MultipleTrackExperiment.txt");
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
        string output = "New Generation: " + to_string(generation) + "\n";
#ifndef CLOUD_COMPUTING
        cout << output;
#else
        neural::FileWriter::stringToFile("./log/MultipleTrackExperiment.txt", output);
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



MultipleTrackExperiment::_Evaluator::_Evaluator(AiSettings ai_settings){

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

MultipleTrackExperiment::_Evaluator::~_Evaluator(){ 
    evaluators.erase(evaluators.begin(), evaluators.end());
}

EvaluationResult MultipleTrackExperiment::_Evaluator::evaluate_network(neat::Network* network) {
    EvaluationResult result = EvaluationResult();

    for (int i = 0; i < evaluators.size(); i++) {
        EvaluationResult er = evaluators[i]->evaluate_network(network);
        er.fitness = neural::fitness_distance_time_exp(er.simResult, evaluators[i]->sim_settings.termination_distance, 300, 20);
        result.fitness += er.fitness;
        result.simResult.distance_on_track += er.simResult.distance_on_track;
        result.simResult.distance_driven += er.simResult.distance_driven;
        result.partialResults.push_back(er);
    }

    return result;
}

void MultipleTrackExperiment::_Evaluator::print(const EvaluationResult& result) {
    string output;

    output += "\nNew Best: \n";
    output += "Total fitness: "   + to_string(result.fitness) + "\n";
    output += "Total distance: "  + to_string(result.simResult.distance_on_track) + "\n";
    output += "Total time: "      + to_string(result.simResult.time_alive) + "\n";

    output += "Evaluator 0, 30 right: \n";
    output += " Fitness:  " + to_string(result.partialResults[0].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[0].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[0].simResult.time_alive) + "\n";

    output += "Evaluator 1, 30 left: \n";
    output += " Fitness:  " + to_string(result.partialResults[1].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[1].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[1].simResult.time_alive) + "\n";

    output += "Evaluator 2, 90 right: \n";
    output += " Fitness:  " + to_string(result.partialResults[2].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[2].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[2].simResult.time_alive) + "\n";

    output += "Evaluator 3, 90 left: \n";
    output += " Fitness:  " + to_string(result.partialResults[3].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[3].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[3].simResult.time_alive) + "\n";

    output += "Evaluator 4, 180 right: \n";
    output += " Fitness:  " + to_string(result.partialResults[3].fitness) + "\n";
    output += " Distance: " + to_string(result.partialResults[3].simResult.distance_on_track) + "\n";
    output += " Time:     " + to_string(result.partialResults[3].simResult.time_alive) + "\n";

    
#ifndef CLOUD_COMPUTING
    cout << output;
#else
    neural::FileWriter::stringToFile("./log/MultipleTrackExperiment.txt", output);
#endif
}

void MultipleTrackExperiment::_Evaluator::reset() {
    for (auto& sim_evaluator : evaluators) {
        sim_evaluator->reset();
    }
}