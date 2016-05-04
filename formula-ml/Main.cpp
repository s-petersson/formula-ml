#define CLOUD_COMPUTING

#include <iostream>
#include <neural/FixedNetwork.h>
#include <neural/FixedNetworkTrainer.h>
#include <experiments/Experiment.h>
#include <experiments/NeatCurveDataExperiment/NeatCurveDataExperiment.h>
#include <experiments/MultipleTrackExperiment/MultipleTrackExperiment.h>
#include <experiments/AlternatingTrackExperiment/AlternatingTrackExperiment.h>
#include <experiments/xor/XORExperiment.h>

#ifndef CLOUD_COMPUTING
#include <experiments/ManualControl/ManualControl.h>
#endif

int main(void) {
    Experiment* experiment = nullptr;
    int chosen_experiment;

    std::cout   << "Experiments available: "    << std::endl;
#ifndef CLOUD_COMPUTING
    std::cout   << "(1)  Manual Control"        << std::endl;
#endif
#ifdef CLOUD_COMPUTING
    std::cout   << "(1)  NOT AVAILABLE"         << std::endl;
#endif
    std::cout   << "(2)  NEAT with curve data"  << std::endl
                << "(3)  Loaded NEAT generation with curve data" << std::endl
                << "(4)  XOR "                  << std::endl
                << "(5)  Multiple tracks "      << std::endl
                << "(6)  Alternating tracks "   << std::endl
                << "(7)  Simple track, 90_10_r" << std::endl
                << "(8)  Shortest distance"     << std::endl
                << "Input the number you want to run: ";
    std::cin >> chosen_experiment;
    switch (chosen_experiment) {
#ifndef CLOUD_COMPUTING
        case 1:
            experiment = new ManualControl();
            break;
#endif
        case 2:
            {
                NeatCurveDataExperiment* e = new NeatCurveDataExperiment();
                experiment = e;


                SimulatorSettings sim_settings = SimulatorSettings();
                sim_settings.track_path = new string("./res/models/circuit_narrow.model");
                sim_settings.completeTrack = true;
                sim_settings.termination_distance = 5200.f;
                sim_settings.max_time = 400.f;

                sim_settings.car_speed_max = 50.f;
                sim_settings.car_speed_initial = 0.f;
				sim_settings.min_avg_speed = 3.f;
				sim_settings.avg_speed_excemption_distance = 140.f;

                e->sim_settings = sim_settings;


                AiSettings ai_settings = AiSettings();
                ai_settings.angle_to_line            = true;
                ai_settings.distance_to_middle       = false;
                ai_settings.distance_to_edges        = true;
                ai_settings.speed                    = true;
                ai_settings.curve_data               = true;
                ai_settings.curve_data_sum_absolutes = true;

                ai_settings.nbr_of_curve_points = 5;
                ai_settings.curve_point_spacing = 15.f;
                ai_settings.curve_point_spacing_incremental_percentage = 0.3f;

                ai_settings.output_speed = true;

                e->ai_settings = ai_settings;
            }
            break;
		case 3:
			{	
				std::string path;
				std::cout << "Abosultue path of designated pool/generation: ";
				std::cin >> path;
				NeatCurveDataExperiment* e = new NeatCurveDataExperiment(path);
				experiment = e;


				SimulatorSettings sim_settings = SimulatorSettings();
				sim_settings.track_path = new string("./res/models/circuit_narrow.model");
				sim_settings.completeTrack = true;
				sim_settings.termination_distance = 5200.f;
				sim_settings.max_time = 400.f;

				sim_settings.car_speed_max = 50.f;
				sim_settings.car_speed_initial = 0.f;
				sim_settings.min_avg_speed = 3.f;
				sim_settings.avg_speed_excemption_distance = 140.f;

				e->sim_settings = sim_settings;


				AiSettings ai_settings = AiSettings();
				ai_settings.angle_to_line = true;
				ai_settings.distance_to_middle = false;
				ai_settings.distance_to_edges = true;
				ai_settings.speed = true;
				ai_settings.curve_data = true;
				ai_settings.curve_data_sum_absolutes = true;

				ai_settings.nbr_of_curve_points = 6;
				ai_settings.curve_point_spacing = 25.f;
				ai_settings.curve_point_spacing_incremental_percentage = 0.3f;

				ai_settings.output_speed = true;

				e->ai_settings = ai_settings;
			}
			break;
        case 4:
            experiment = new XORExperiment();
            break;
        case 5:
            experiment = new MultipleTrackExperiment();
            break;
        case 6:
            experiment = new AlternatingTrackExperiment();
            break;
        case 7:   // Simple track
            {
                NeatCurveDataExperiment* e = new NeatCurveDataExperiment();
                experiment = e;

                SimulatorSettings sim_settings = SimulatorSettings();
                sim_settings.track_path = new string("./res/models/corners/corner_90_10_r.model");
                sim_settings.completeTrack = false;
                sim_settings.termination_distance = 850.f; // guessed
                sim_settings.max_time = 400.f;

                sim_settings.car_speed_max = 50.f;
                sim_settings.car_speed_initial = 0.f;
                sim_settings.min_avg_speed = 3.f;
                sim_settings.avg_speed_excemption_distance = 140.f;

                e->sim_settings = sim_settings;


                AiSettings ai_settings = AiSettings();
                ai_settings.angle_to_line = true;
                ai_settings.distance_to_middle = false;
                ai_settings.distance_to_edges = true;
                ai_settings.speed = true;
                ai_settings.curve_data = true;
                ai_settings.curve_data_sum_absolutes = true;

                ai_settings.nbr_of_curve_points = 5;
                ai_settings.curve_point_spacing = 15.f;
                ai_settings.curve_point_spacing_incremental_percentage = 0.3f;

                ai_settings.output_speed = true;

                e->ai_settings = ai_settings;
            }
            break;
        case 8:   // Shortest distance
            {
                NeatCurveDataExperiment* e = new NeatCurveDataExperiment();
                experiment = e;

                SimulatorSettings sim_settings = SimulatorSettings();
                sim_settings.track_path = new string("./res/models/circuit_wide.model");
                sim_settings.completeTrack = true;
                sim_settings.termination_distance = 5200; // guessed
                sim_settings.max_time = 700.f;

                sim_settings.car_speed_max = 15.f;
                sim_settings.car_speed_initial = 10.f;

                e->sim_settings = sim_settings;


                AiSettings ai_settings = AiSettings();
                ai_settings.angle_to_line       = true;
                ai_settings.distance_to_middle  = true;
                ai_settings.distance_to_edges   = true;
                ai_settings.speed               = false;
                ai_settings.curve_data          = true;
                ai_settings.curve_data_sum_absolutes = true;

                ai_settings.nbr_of_curve_points = 5;
                ai_settings.curve_point_spacing = 15.f;
                ai_settings.curve_point_spacing_incremental_percentage = 0.3f;

                ai_settings.output_speed = false;

                e->ai_settings = ai_settings;
            }
            break;
		default:
#ifndef CLOUD_COMPUTING
			experiment = new ManualControl();
#else
            experiment = new XORExperiment();
#endif
			break;
    }

    experiment->run();
	delete experiment;
	return 0;
}