#ifndef SIM_SIMULATOR
#define SIM_SIMULATOR

#include <sim/model/CarModel.h>
#include <sim/model/TrackModel.h>

class Simulator {
public:
	Simulator(CarModel * car, TrackModel * track);
	~Simulator();
	void update(float dt);

	CarModel * car;
	TrackModel * track;
};
#endif