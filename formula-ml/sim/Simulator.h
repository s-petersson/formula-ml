#ifndef SIM_SIMULATOR
#define SIM_SIMULATOR
#include "Car.h"
#include "Track.h"

class Simulator {
public:
	Simulator();
	~Simulator();
	void update(float dt);
private:
	Car * car;
	Track * track;
};
#endif