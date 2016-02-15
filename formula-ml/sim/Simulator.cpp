#include "Simulator.h"
#include <neural/FixedNetwork.h>
using namespace neural;

Simulator::Simulator(CarModel * _car, TrackModel * _track) {
	car = _car;
	track = _track;
    network = new FixedNetwork;

    for (int i = -5; i < 6; i++) {
        printf("%f \n", sigmoid(i));
    }
}

Simulator::~Simulator() {
	if (car) delete car;
	if (track) delete track;
    if (network) delete network;
}

void Simulator::update(float dt) {

    NetworkIO in, out;
    // Fill 'in' with data
    //out = network->fire(in);
    // use out to update car.
	car->update(dt);
    if (track->onTrack(car->position)) {
        printf("Car on Track! \n");
    }
}