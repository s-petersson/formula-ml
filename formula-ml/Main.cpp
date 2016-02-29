#include <iostream>
#include <neural/FixedNetwork.h>
#include <neural/FixedNetworkTrainer.h>
#include <experiments/Experiment.h>
#include <experiments/ManualControl.h>
#include <experiments/FixedNetworkMidline.h>
#include <experiments/NEATNetworkMidline.h>

int main(void) {
    Experiment* experiment;
    int chosen_experiment;
    std::cout   << "Experiments available: "    << std::endl
                << "(1) Manual Control"         << std::endl
                << "(2) Fixed Midline"          << std::endl
                << "(3) NEAT Midline"           << std::endl
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
            experiment = new NEATNetworkMidline();
            break;
		default:
			experiment = new ManualControl();
			break;
    }
    experiment->run();

	return 0;
}