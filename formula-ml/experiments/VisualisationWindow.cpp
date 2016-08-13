#include "VisualisationWindow.h"
#include <core/Keyboard.h>

VisualisationWindow::VisualisationWindow(Simulator * simulator, std::shared_ptr<RacelineLogger> rl) {
    window = new Window();

    state = new VisualisationState(rl);
    state->simulator = simulator;
    state->renderers.push_back(new StandardRenderer(simulator));
    window->setState(state);
}

VisualisationWindow::~VisualisationWindow() {
    if (state) delete state;
    if (window) delete window;
}

void VisualisationWindow::run() {
    window->run();
}

void VisualisationWindow::setBestNetwork (neat::Network * network) {
    state->bestNetwork = network;
    if (state->network_view) delete state->network_view;
    state->network_view = new NetworkView(network);
}

VisualisationWindow::VisualisationState::VisualisationState(std::shared_ptr<RacelineLogger> rl) {
    network_view = nullptr;
    raceline_logger = rl;
}

VisualisationWindow::VisualisationState::~VisualisationState() {
    if (simulator) delete simulator;
    if (network_view) delete network_view;
    if (bestNetwork) delete bestNetwork;
}

void VisualisationWindow::VisualisationState::reset() {
    simulator->reset();
    for (auto&& r : renderers) {
        r->reset();
    }
    if (this->bestNetwork) {
        this->bestNetwork->reset();
    }
    raceline_logger->process_jobs();
    
}

void VisualisationWindow::VisualisationState::run(float dt) {
    // Possibly reset
    if (isKeyDown(GLFW_KEY_HOME)) {
        reset();
    }

    // Update simulator
    for (int i = 0; i < 10; i++) {
        if (simulator->has_terminated()) {
            reset();
        }
        simulator->update(dt);
    }
    // Render
    for (auto && renderer : renderers) {
        renderer->render();
    }

    if (network_view) {
        network_view->render();
    }
}
