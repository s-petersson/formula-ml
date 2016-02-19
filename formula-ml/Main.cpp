#include <core/Window.h>
#include <neural/FixedNetwork.h>
#include "SimulationState.h"

// 0 = Simulator with keyboard control
// 1 = AI running the simulator
// 2 = AI training
// 3 = AI running XOR
#define EXPERIMENT 0

int main(void) {
	Window * window = new Window;

    if (EXPERIMENT == 0 || EXPERIMENT == 1) {
        Simulator * sim = new Simulator();
        // Create simulated objects
        // NOTE: Starting grid is at first "checkpoint". In order
        //       to change this, offset the checkpoint order.
        sim->track = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));
        sim->car = new CarModel();

        // Place car at the tracks starting grid.
        sim->car->position = sim->track->get_start_grid_pos();
        sim->network = new neural::FixedNetwork(2, 1, 1, 2);
        sim->progress_timeout = 5;

        SimulationState * s = new SimulationState(sim);
        window->setState(s);
        window->run();
        delete window;
    } else if (EXPERIMENT == 2) {
        // Train the AI
    } else if (EXPERIMENT == 3) {
        // Run XOR experiment
    }

	return 0;
}