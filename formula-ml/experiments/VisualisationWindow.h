#ifndef VisualisationWindow_h
#define VisualisationWindow_h

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
#include <experiments/RacelineLogger.h>
#include <mutex>
#include <memory>

class VisualisationWindow {
public:
    VisualisationWindow(Simulator * simulator, std::shared_ptr<RacelineLogger> rl);
    ~VisualisationWindow();

    void run();

    void setBestNetwork(neat::Network * network);
private:
    class VisualisationState : public WindowState {
    public:
        VisualisationState(std::shared_ptr<RacelineLogger> rl);
        ~VisualisationState();

        void run(float dt);
        void reset();

        Simulator* simulator;
        neat::Network * bestNetwork;

        vector<Renderer*> renderers;
        NetworkView* network_view;
        std::shared_ptr<RacelineLogger> raceline_logger;
    };
    Window * window;
    VisualisationState * state;
};

#endif
