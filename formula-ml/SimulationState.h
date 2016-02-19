#ifndef SIMULATION_STATE
#define SIMULATION_STATE

#include <core/WindowState.h>
#include <sim/Simulator.h>
#include <core/util/Camera.h>
#include <sim/view/CarView.h>
#include <sim/view/TrackView.h>
#include <sim/view/GridView.h>

struct Grid;

class SimulationState : public WindowState {
public:
	SimulationState(Simulator* simulator);
	~SimulationState();

	void update(float dt);
	void render();

private:
	Simulator * sim;
    Camera * camera;

    GridView * gridView;
    TrackView *trackView;
    CarView *carView;
};

#endif