#include <iostream>
#include "ManualControl.h"

ManualControl::ManualControl() {
    simulator       = new Simulator();
    window          = new Window();

    simulator->progress_timeout = 5;
    simulator->track            = new TrackModel(glm::vec3(35.169220,
                                                           -702.223755,
                                                           5.000004));
    simulator->car              = new CarModel();
    simulator->car->position    = simulator->track->get_start_grid_pos();

    simulationState = new SimulationState(simulator);
    window->setState(simulationState);

    // Set input values
    const int TRACK_GRID_WIDTH  = 10;
    const int TRACK_GRID_DEPTH  = 18;
    const int OTHER_INPUTS      = 1;

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
    window->run();
}

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

        return control;
    };
}