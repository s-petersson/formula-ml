#ifndef SIM_SIMULATOR
#define SIM_SIMULATOR

#include <sim/model/CarModel.h>
#include <sim/model/TrackModel.h>

#include <neural/Neural.h>
#include <functional>


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
	TrackModel * track;
	float progress_timeout;
    std::function<CarControl()> carUpdater;

	// Readable by user:
	SimulationResult result;

    float distance_to_middle();
    float angle_to_line();

private:
};
#endif