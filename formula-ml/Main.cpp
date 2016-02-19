#include <core/Window.h>
#include <neural/FixedNetwork.h>
#include <core/Keyboard.h>
#include "SimulationState.h"

// 0 = Simulator with keyboard control
// 1 = AI running the simulator
// 2 = AI training
// 3 = AI running XOR
int EXPERIMENT = 0;

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
        sim->progress_timeout = 5;

        // Set input values
        const int TRACK_GRID_WIDTH = 10;
        const int TRACK_GRID_DEPTH = 18;
        const int OTHER_INPUTS = 1;

        neural::NetworkIO network_indata = neural::NetworkIO();
        network_indata.value_count = OTHER_INPUTS + TRACK_GRID_WIDTH * TRACK_GRID_DEPTH;
        network_indata.values = new float[network_indata.value_count];

        TrackGrid grid = TrackGrid();
        grid.data = &network_indata.values[OTHER_INPUTS];
        grid.size = TRACK_GRID_WIDTH * TRACK_GRID_DEPTH;
        grid.width = TRACK_GRID_WIDTH;
        grid.depth = TRACK_GRID_DEPTH;
        grid.cell_size = 2.5;
        grid.value_track = 1;
        grid.value_not_track = 0;

        if (EXPERIMENT == 0) { // Keyboard control
            sim->carUpdater = [&]() {
                sim->track->fillTrackGrid(grid, sim->car->position, sim->car->direction);

                CarControl control = CarControl();

                bool gas = isKeyDown(GLFW_KEY_UP);
                bool brake = isKeyDown(GLFW_KEY_DOWN);
                bool steerLeft = isKeyDown(GLFW_KEY_LEFT);
                bool steerRight = isKeyDown(GLFW_KEY_RIGHT);
                bool steerCareful = isKeyDown(GLFW_KEY_RIGHT_CONTROL);
                bool accelerateMax = isKeyDown(GLFW_KEY_RIGHT_SHIFT);

                if (gas) {
                    control.acceleration = accelerateMax ? 1 : 0.5f;
                }
                if (brake) {
                    control.brake = accelerateMax ? 1 : 0.5f;
                }

                if (steerLeft && !steerRight) {
                    control.steer = steerCareful ? 0.5f : 1;
                } else if (!steerLeft && steerRight) {
                    control.steer = steerCareful ? -0.5f : -1;
                }
                return control;
            };
        } else { // AI Control
            neural::Network* network = new neural::FixedNetwork(2, 1, 1, 2);

            sim->carUpdater = [&]() {
                CarControl control = CarControl();

                // Car data
                network_indata.values[0] = sim->car->getSpeed();

                // Track data
                sim->track->fillTrackGrid(grid, sim->car->position, sim->car->direction);

                // Fire network
                neural::NetworkIO out = network->fire(network_indata);

                // Use network output
                // Assume 'out' to be of correct size and ranges


                control.acceleration = out.values[0];
                control.brake = out.values[1];
                control.steer = out.values[2];

                return control;
            };
        }

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