#include <iostream>
#include <neural/FixedNetwork.h>
#include <neural/FixedNetworkTrainer.h>
#include <experiments/Experiment.h>
#include <experiments/ManualControl/ManualControl.h>
#include <experiments/FixedNetworkMidline/FixedNetworkMidline.h>
#include <experiments/NeatCurveDataExperiment/NeatCurveDataExperiment.h>
#include <experiments/xor/XORExperiment.h>

int main(void) {
    Experiment* experiment;
    int chosen_experiment;
    std::cout   << "Experiments available: "    << std::endl
                << "(1) Manual Control"         << std::endl
                << "(2) Fixed Midline"          << std::endl
                << "(3) NEAT with curve data"   << std::endl
				<< "(4) Loaded NEAT generation with curve data" << std::endl
                << "(5) XOR " << std::endl
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
                e->nbr_of_curve_points = 5;
                e->curve_point_spacing = 15.f;
                e->curve_point_spacing_incremental_percentage = 0.3f;
                e->max_time = 400.f;
                e->termination_distance = 5200.f;
                e->car_speed = 15.f;
            }
            break;
		case 4:
			{	
				std::string path;
				std::cout << "Abosultue path of designated pool/generation: ";
				std::cin >> path;
				//NeatCurveDataExperiment* e = new NeatCurveDataExperiment("C:\\Users\\Daniel\\code\\formula-ml\\formula-ml\\saves\\Generation 19");
				NeatCurveDataExperiment* e = new NeatCurveDataExperiment(path);
				experiment = e;
				e->nbr_of_curve_points = 10;
				e->max_time = 400.f;
				e->termination_distance = 5200.f;
                e->car_speed = 15.f;
			}
			break;
        case 5:
            //experiment = new XORExperiment();
            experiment = new XORExperiment();
            break;
		default:
			experiment = new ManualControl();
			break; 
    }

    experiment->run();
	delete experiment;
	return 0;
}