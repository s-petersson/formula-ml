#include <iostream>
#include <neural/FixedNetwork.h>
#include <neural/FixedNetworkTrainer.h>
#include <experiments/Experiment.h>
#include <experiments/ManualControl/ManualControl.h>
#include <experiments/FixedNetworkMidline/FixedNetworkMidline.h>
#include <experiments/NeatCurveDataExperiment/NeatCurveDataExperiment.h>
#include <experiments/MultipleTrackExperiment/MultipleTrackExperiment.h>
#include <experiments/AlternatingTrackExperiment/AlternatingTrackExperiment.h>
#include <experiments/xor/XORExperiment.h>

int main(void) {
    Experiment* experiment = nullptr;
    int chosen_experiment;
    std::cout   << "Experiments available: "    << std::endl
                << "(1) Manual Control"         << std::endl
                << "(2) Fixed Midline"          << std::endl
                << "(3) NEAT with curve data"   << std::endl
				<< "(4) Loaded NEAT generation with curve data" << std::endl
                << "(5) XOR " << std::endl
                << "(6) Multiple tracks " << std::endl
                << "(7) Alternating tracks " << std::endl
                << "Input the number you want to run: ";
    std::cin >> chosen_experiment;
    switch (chosen_experiment) {
        case 1:
            experiment = new ManualControl();
            break;
        case 2:
            experiment = new FixedNetworkMidline();
            break;
        case 3:
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
		case 4:
			{	
				std::string path;
				std::cout << "Abosultue path of designated pool/generation: ";
				std::cin >> path;
				//NeatCurveDataExperiment* e = new NeatCurveDataExperiment("C:\\Users\\Daniel\\code\\formula-ml\\formula-ml\\saves\\Generation 19");
				NeatCurveDataExperiment* e = new NeatCurveDataExperiment(path);


                SimulatorSettings sim_settings = SimulatorSettings();
                sim_settings.track_path = new string("./res/models/circuit_narrow.model");
                sim_settings.completeTrack = true;
                sim_settings.termination_distance = 5200.f;
                sim_settings.max_time = 400.f;

                sim_settings.car_speed_max = 15.f;
                sim_settings.car_speed_initial = 0.f;

				sim_settings.min_avg_speed = 3.f;
				sim_settings.avg_speed_excemption_distance = 140.f;

                e->sim_settings = sim_settings;


                AiSettings ai_settings;
                ai_settings.angle_to_line = true;
                ai_settings.distance_to_middle       = false;
                ai_settings.distance_to_edges        = true;
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
        case 5:
            //experiment = new XORExperiment();
            experiment = new XORExperiment();
            break;
        case 6:
            experiment = new MultipleTrackExperiment();
            break;
        case 7:
            experiment = new AlternatingTrackExperiment();
            break;
		default:
			experiment = new ManualControl();
			break; 
    }

    experiment->run();
	delete experiment;
	return 0;
}