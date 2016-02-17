#ifndef SIM_SIMULATOR
#define SIM_SIMULATOR

#include <sim/model/CarModel.h>
#include <sim/model/TrackModel.h>

#include <neural/Neural.h>


struct SimulationResult {
	float distance_driven;
	float time_alive;
};

class Simulator {
public:
	Simulator();
	~Simulator();
	void update(float dt);
	SimulationResult run(float dt);

	CarModel * car;
	TrackModel * track;
	SimulationResult result;
	float progress_timeout;

private:
    neural::Network* network;
	neural::NetworkIO network_indata;
	TrackGrid grid;
};
#endif