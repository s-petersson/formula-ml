#ifndef SIMULATION_STATE
#define SIMULATION_STATE

#include <core/WindowState.h>
#include <sim/Simulator.h>
#include <core/util/OrthoCamera.h>

class SimulationState : public WindowState {
public:
	SimulationState();
	~SimulationState();

	void update(float dt);
	void render();

private:
	Simulator * sim;
    OrthoCamera * camera;
};

#endif