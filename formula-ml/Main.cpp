#include <iostream>
#include <neural/FixedNetwork.h>
#include <neural/FixedNetworkTrainer.h>
#include <experiments/Experiment.h>
#include <experiments/ManualControl/ManualControl.h>
#include <experiments/FixedNetworkMidline/FixedNetworkMidline.h>
#include <experiments/NeatCurveDataExperiment/NeatCurveDataExperiment.h>

int main(void) {
    Experiment* experiment;
    int chosen_experiment;
    std::cout   << "Experiments available: "    << std::endl
                << "(1) Manual Control"         << std::endl
                << "(2) Fixed Midline"          << std::endl
                << "(3) NEAT with curve data"   << std::endl
				<< "(4) Loaded NEAT generation with curve data" << std::endl
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
                e->nbr_of_curve_points = 10;
                e->max_time = 2000.f;
                e->termination_distance = 5700.f;
            }
            break;
		case 4:
			{
				NeatCurveDataExperiment* e = new NeatCurveDataExperiment();
				experiment = e;
				e->nbr_of_curve_points = 10;
				e->max_time = 2000.f;
				e->termination_distance = 5700.f; 
			}
		default:
			experiment = new ManualControl();
			break;
    }
    experiment->run();

	return 0;
}