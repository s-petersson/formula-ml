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

	// To be set by the user before start:
	CarModel * car;
	neural::Network* network;
	TrackModel * track;
	float progress_timeout;

	// Readable by user:
	SimulationResult result;

private:
	neural::NetworkIO network_indata;
	TrackGrid grid;
};
#endif