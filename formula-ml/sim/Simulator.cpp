#include "Simulator.h"
#include <neural/FixedNetwork.h>
#include "core/Keyboard.h"
using namespace neural;

Simulator::Simulator(CarModel * _car, TrackModel * _track) {
	car = _car;
	track = _track;
    network = new FixedNetwork;
}

Simulator::~Simulator() {
	if (car) delete car;
	if (track) delete track;
    if (network) delete network;
}

void Simulator::update(float dt) {

    NetworkIO in;
    in.values = new float[2];
    in.value_count = 2;
    // Fill 'in' with data
    NetworkIO out = network->fire(in);
    // use out to update car.

	bool gas = isKeyDown(GLFW_KEY_UP);
	bool brake = isKeyDown(GLFW_KEY_DOWN);
	bool steerLeft = isKeyDown(GLFW_KEY_LEFT);
	bool steerRight = isKeyDown(GLFW_KEY_RIGHT);
	bool steerCareful = isKeyDown(GLFW_KEY_RIGHT_CONTROL);
	bool accelerateMax = isKeyDown(GLFW_KEY_RIGHT_SHIFT);

	CarControl control = CarControl();

	if (gas) {
		control.acceleration = accelerateMax ? 1 : 0.5;
	}
	if (brake) {
		control.brake = accelerateMax ? 1 : 0.5;
	}

	if (steerLeft && !steerRight) {
		control.steer = steerCareful ? 0.5 : 1;
	} else if (!steerLeft && steerRight) {
		control.steer = steerCareful ? -0.5 : -1;
	}

	car->update(dt, control);
    if (track->onTrack(car->position)) {
        printf("Car on Track! \n");
    }
}