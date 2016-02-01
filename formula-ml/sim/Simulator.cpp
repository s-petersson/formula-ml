#include "Simulator.h"

Simulator::Simulator() {
	car = new Car();
	track = new Track();
}

Simulator::~Simulator() {
	if (car) delete car;
	if (track) delete track;
}

void Simulator::update(float dt) {
	car->update(dt);
}