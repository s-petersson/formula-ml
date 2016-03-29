#include <iostream>
#include <sim/SimulationState.h>
#include "ManualControl.h"
#include <experiments/StandardRenderer.h>
#include <core/gfx/Renderer.h>

// Set input values
const int TRACK_GRID_WIDTH  = 10;
const int TRACK_GRID_DEPTH  = 18;
const int OTHER_INPUTS      = 1;

ManualControl::ManualControl() {
    simulator                       = new Simulator();
    window                          = new Window();

    simulator->progress_timeout     = 5;
    simulator->track                = new TrackModel(glm::vec3(0,0,0));
    simulator->car                  = new CarModel(simulator->track->get_start_grid_pos(), glm::vec3(0, 1, 0), 15.f);

    std::vector<Renderer*> renderers;

	renderers.push_back(new StandardRenderer(simulator));
    simulationState                 = new SimulationState(simulator, renderers);
    window->setState(simulationState);

    neural::NetworkIO network_indata    = neural::NetworkIO();
    network_indata.value_count          = OTHER_INPUTS + TRACK_GRID_WIDTH * TRACK_GRID_DEPTH;
    network_indata.values               = new float[network_indata.value_count];

    grid                    = new TrackGrid();
    grid->data              = &network_indata.values[OTHER_INPUTS];
    grid->size              = TRACK_GRID_WIDTH * TRACK_GRID_DEPTH;
    grid->width             = TRACK_GRID_WIDTH;
    grid->depth             = TRACK_GRID_DEPTH;
    grid->cell_size         = 2.5;
    grid->value_track       = 1;
    grid->value_not_track   = 0;

    simulator->carUpdater = updater();
}

ManualControl::~ManualControl() {
    delete window;
    delete simulationState;
    delete grid;
}

void ManualControl::run() {
    // Display the window and run it's loop.
    // This will run SimulationState update/render.
    window->run();
}

int print_counter = 0;

std::function<CarControl()> ManualControl::updater() {
    return [&]() {
        simulator->track->fillTrackGrid(*grid,
                                        simulator->car->position,
                                        simulator->car->direction);

        const bool gas              = isKeyDown(GLFW_KEY_UP);
        const bool brake            = isKeyDown(GLFW_KEY_DOWN);
        const bool steerLeft        = isKeyDown(GLFW_KEY_LEFT);
        const bool steerRight       = isKeyDown(GLFW_KEY_RIGHT);
        const bool steerCareful     = isKeyDown(GLFW_KEY_RIGHT_CONTROL);
        const bool accelerateMax    = isKeyDown(GLFW_KEY_RIGHT_SHIFT);

        CarControl control = CarControl();
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
        /*
        // If this is removed, also remove the global variable print_counter
        if (print_counter++ % 100 == 0) {
            float data[10];
            int i = 0;
            simulator->write_track_curve(&data[0], i, 10);
            cout << "Track curve data: \n"
                << "counter i incremented to " << i << "\n"
                << data[0] << "\n"
                << data[1] << "\n"
                << data[2] << "\n"
                << data[3] << "\n"
                << data[4] << "\n"
                << data[5] << "\n"
                << data[6] << "\n"
                << data[7] << "\n"
                << data[8] << "\n"
                << data[9] << "\n\n\n";
        }
        /**/
        return control;
    };
}