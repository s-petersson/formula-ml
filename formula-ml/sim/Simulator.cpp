#include "Simulator.h"
#include <neural/FixedNetwork.h>
#include <iostream>
#include "core/Keyboard.h"

using namespace neural;

Simulator::Simulator() {
    // Set input values
    const int TRACK_GRID_WIDTH = 10;
    const int TRACK_GRID_DEPTH = 18;
    const int OTHER_INPUTS = 1;

    network_indata = NetworkIO();
    network_indata.value_count = OTHER_INPUTS + TRACK_GRID_WIDTH * TRACK_GRID_DEPTH;
    network_indata.values = new float[network_indata.value_count];

	result = SimulationResult();
	progress_timeout = 0;

    grid = TrackGrid();
    grid.data = &network_indata.values[OTHER_INPUTS];
    grid.size = TRACK_GRID_WIDTH * TRACK_GRID_DEPTH;
    grid.width = TRACK_GRID_WIDTH;
    grid.depth = TRACK_GRID_DEPTH;
    grid.cell_size = 2.5;
    grid.value_track = 1;
    grid.value_not_track = 0;
}

Simulator::~Simulator() {
	if (car) delete car;
	if (track) delete track;
    if (network) delete network;
}

/*
	Run a complete simulation until failure
	Each simulation step update with time dt
*/
SimulationResult Simulator::run(const float dt) {
	SimulationResult best = SimulationResult();

	while (true) {
		update(dt);

		// Check for crash
		if (!track->on_track(car->position)) {
			// Call it quits
			break;
		}

		// Check for progress
		if (result.distance_driven > best.distance_driven) {
			// The car has progressed
			best = result;
		} else if(result.time_alive > best.time_alive + progress_timeout) {
			// No progress for a while
			// Call it quits
			break;
		}
	}
	return result;
}

/*
	Update the simulation with one time step dt [seconds]
*/
void Simulator::update(float dt) {
	// Update result
	result.time_alive += dt;
	// TODO: increment distance on track

	// Car data
	network_indata.values[0] = car->getSpeed();

	// Track data
	track->fillTrackGrid(grid, car->position, car->direction);

	// Fire network
	/*
    NetworkIO out = network->fire(in);
	*/

	// Use network output
	// Assume 'out' to be of correct size and ranges
	CarControl control = CarControl();
	/*
	control.acceleration = out.values[0];
	control.brake = out.values[1];
	control.steer = out.values[2];
	*/

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

    Checkpoint next_checkpoint = checkpoints[car->checkpoint];
    glm::vec3 car_p = car->position - next_checkpoint.left;
    glm::vec3 gate = glm::normalize(next_checkpoint.left - next_checkpoint.right);
    glm::vec3 point_on_gate = glm::dot(car_p, gate) * gate;

    if (glm::length(car_p - point_on_gate) < 0.5) {
        car->distance_on_track = checkpoints[car->checkpoint].distance_on_track;
        car->checkpoint++;
    } else {
        int last_checkpoint = glm::max(car->checkpoint - 1, 0);
        float d = checkpoints[last_checkpoint].distance_on_track + glm::distance(car->position, checkpoints[last_checkpoint].middle);
        car->distance_on_track = d;
    }

	car->update(dt, control);
    if (track->on_track(car->position)) {
        // On model.
    }
}