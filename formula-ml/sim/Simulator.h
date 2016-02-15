#ifndef SIM_SIMULATOR
#define SIM_SIMULATOR

#include <sim/model/CarModel.h>
#include <sim/model/TrackModel.h>

#include <neural/Neural.h>

class Simulator {
public:
	Simulator(CarModel * car, TrackModel * track);
	~Simulator();
	void update(float dt);

	CarModel * car;
	TrackModel * track;

private:
    neural::Network* network;
};
#endif