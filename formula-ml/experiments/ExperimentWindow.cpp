#include "ExperimentWindow.h"
#include <core/Keyboard.h>
#include <core/util/Util.h>

/* ExperimentWindow */

ExperimentWindow::ExperimentWindow(Simulator* simulator, std::shared_ptr<neat::Trainer> _trainer, std::shared_ptr<RacelineLogger> rl) {
    window = new Window();
    state = new ExperimentState(rl);

	state->simulator = simulator;
    window->setState(state);
    state->trainer = _trainer;
    add_renderer(new StandardRenderer(simulator));
}
ExperimentWindow::~ExperimentWindow() {
    if (state) delete state;
    if (window) delete window;
}

void ExperimentWindow::run() {
    window->run();
}

void ExperimentWindow::setNetworkLocation(neat::Network** network, bool delayed_update) {
    state->network_mutex.lock();

    state->network_location = network;
    state->delayed_update = delayed_update;

    state->network_mutex.unlock();
}

void ExperimentWindow::add_renderer(Renderer* r) {
    state->renderers.push_back(r);
}
void ExperimentWindow::clear_renderers() {
    state->renderers.clear();
}


/* ExperimentState helper class */

ExperimentWindow::ExperimentState::ExperimentState(std::shared_ptr<RacelineLogger> rl) {
    network_view = nullptr;
    network_location = nullptr;
	raceline_logger = rl;
}

ExperimentWindow::ExperimentState::~ExperimentState() {
    if (simulator) delete simulator;
    if (network_view) delete network_view;
}

void ExperimentWindow::ExperimentState::reset() {
    simulator->reset();
	for (auto&& r : renderers) {
		r->reset();
	}
    if (trainer) {
        network_mutex.lock();
        if (network_location) {
            if (*network_location) delete *network_location;
            *network_location = new neat::Network(trainer->get_best().genes);
           
            if (network_view) delete network_view;
            network_view = new NetworkView(*network_location);

            //network_buffer = nullptr;
        }

        if (network_location && *network_location) {
            (*network_location)->reset();
        }
        network_mutex.unlock();
    }
	raceline_logger->process_jobs();

}

void ExperimentWindow::ExperimentState::run(float dt) {
    // Possibly reset
    network_mutex.lock();
    bool update_network = !delayed_update;
    network_mutex.unlock();
    if (update_network || isKeyDown(GLFW_KEY_HOME)) {
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
