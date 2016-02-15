#include "Simulator.h"
#include <neural/FixedNetwork.h>
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
	car->update(dt);
    if (track->onTrack(car->position)) {
        printf("Car on Track! \n");
    }
}