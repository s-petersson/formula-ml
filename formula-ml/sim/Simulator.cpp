#include "Simulator.h"

Simulator::Simulator(CarModel * _car, TrackModel * _track) {
	car = _car;
	track = _track;
}

Simulator::~Simulator() {
	if (car) delete car;
	if (track) delete track;
}

void Simulator::update(float dt) {
	car->update(dt);
    if (track->onTrack(car->position)) {
        printf("Car on Track! \n");
    }
}