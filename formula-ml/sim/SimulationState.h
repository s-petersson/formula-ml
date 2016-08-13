#ifndef SIMULATION_STATE
#define SIMULATION_STATE

#include <core/WindowState.h>
#include <sim/Simulator.h>
#include <core/util/Camera.h>
#include <sim/view/CarView.h>
#include <sim/view/TrackView.h>
#include <sim/view/GridView.h>
#include <core/gfx/Renderer.h>

struct Grid;

class SimulationState : public WindowState {
public:
	SimulationState(Simulator * simulator, vector<Renderer*> renderers);
	~SimulationState();

	void run(float dt);
	void add_renderer(Renderer* r);
	void clear_renderers();

private:
	Simulator * simulator;
    vector<Renderer*> renderers;
	Simulator * sim;
    Camera * camera;

    GridView * gridView;
    TrackView *trackView;
    CarView *carView;
};

#endif