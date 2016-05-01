#include <experiments/StandardRenderer.h>
#include "FixedNetworkMidline.h"

FixedNetworkMidline::FixedNetworkMidline() {
    simulator       = new Simulator();
    window          = new Window();

    simulator->carUpdater = updater();

    simulator->progress_timeout = 0.1f;
    simulator->track            = new TrackModel(glm::vec3(35.169220,
                                                           -702.223755,
                                                           5.000004),
                                                 "./res/models/circuit_narrow.model",
                                                 true);
    simulator->car              = new CarModel(simulator->track->get_start_grid_pos(), glm::vec3(), 0.f, 0.f, 15.f);
    simulator->car->setSpeed(15.f);

    vector<Renderer*> renderers;
    renderers.push_back(new StandardRenderer(simulator));
    simulationState                 = new SimulationState(simulator, renderers);
    window->setState(simulationState);

    trainer.run();
    trainer.buildNetwork(trainer.bestGenome);

    network_indata              = neural::NetworkIO();
    network_indata.value_count  = 2;
    network_indata.values       = new float[network_indata.value_count];

    network_output              = neural::NetworkIO();
    network_output.value_count  = 1;
    network_output.values       = new float;
}

FixedNetworkMidline::~FixedNetworkMidline() {
    delete network_indata.values;
    delete window;
    delete simulationState;
}

void FixedNetworkMidline::run() {
    window->run();
}

std::function<CarControl()> FixedNetworkMidline::updater() {
    return [&](){
        trainer.setLineData(&network_indata, 0, simulator);
        trainer.network->fire(network_indata, network_output);
        return trainer.makeCarControl(network_output);
    };
}