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
#include <neural/neat/Trainer.h>
#include <mutex>
#include <memory>

struct Grid;

class ExperimentWindow {
public:
    ExperimentWindow(Simulator* simulator, std::shared_ptr<neat::Trainer> _trainer);
    ~ExperimentWindow();

    void run();

    void add_renderer(Renderer* r);
    void clear_renderers();

    void setNetworkLocation(neat::Network** network, bool delayed_update); // If called, only before run()

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
        std::mutex network_mutex;

        std::shared_ptr<neat::Trainer> trainer;
        vector<Renderer*> renderers;
        NetworkView* network_view;
    };
    Window* window;
    ExperimentState* state;
};

#endif