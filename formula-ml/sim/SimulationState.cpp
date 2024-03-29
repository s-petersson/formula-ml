#include "SimulationState.h"

SimulationState::SimulationState(Simulator * simulator, vector<Renderer*> renderers) {
    this->simulator = simulator;
    this->renderers = renderers;
}


SimulationState::~SimulationState() {
	if (simulator)  delete simulator;
}

void SimulationState::run(float dt) {
    for (int i = 0; i < 10; i++) {
        simulator->update(dt);
    }

    for(auto && renderer : renderers) {
        renderer->render();
    }
}

void SimulationState::add_renderer(Renderer* r) {
	renderers.push_back(r);
}
void SimulationState::clear_renderers() {
	renderers.clear();
}