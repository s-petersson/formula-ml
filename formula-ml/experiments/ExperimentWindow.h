#ifndef EXPERIMENT_STATE
#define EXPERIMENT_STATE

#include <core/WindowState.h>
#include <core/gfx/Renderer.h>
#include <core/util/Camera.h>
#include <sim/Simulator.h>
#include <sim/view/CarView.h>
#include <sim/view/GridView.h>
#include <sim/view/TrackView.h>
#include <core/Window.h>
#include <experiments/StandardRenderer.h>
#include <neural/neat/Network.h>
#include <experiments/NetworkView.h>
#include <mutex>


struct Grid;

class ExperimentWindow {
public:
    ExperimentWindow(Simulator* simulator);
    ~ExperimentWindow();

    void run();

    void add_renderer(Renderer* r);
    void clear_renderers();

    void setNetworkLocation(neat::Network** network, bool delayed_update); // If called, only before run()
    void updateNetwork(neat::Network* network); // Takes memory responsibility of network. Must only be called after setNetworkLocation()

private:
    class ExperimentState : public WindowState {
    public:
        ExperimentState();
        ~ExperimentState();

        void run(float dt);
        void reset();

        Simulator* simulator;

        bool delayed_update;
        neat::Network** network_location;
        neat::Network* network_buffer;
        std::mutex network_mutex;

        vector<Renderer*> renderers;
        NetworkView* network_view;
    };

    Window* window;
    ExperimentState* state;
};

#endif