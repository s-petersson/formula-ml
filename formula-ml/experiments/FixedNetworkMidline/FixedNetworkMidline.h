#ifndef FORMULA_ML_FIXEDNETWORKMIDLINE_H
#define FORMULA_ML_FIXEDNETWORKMIDLINE_H

#include <core/Window.h>
#include <sim/SimulationState.h>
#include <neural/FixedNetworkTrainer.h>
#include <experiments/Experiment.h>

class FixedNetworkMidline : public Experiment {
public:
    FixedNetworkMidline();
    ~FixedNetworkMidline();

    virtual void run();
    std::function<CarControl()> updater();

private:
    SimulationState* simulationState;
    Window* window;
    neural::FixedNetworkTrainer trainer;
    neural::NetworkIO network_indata;
    neural::NetworkIO network_output;
};


#endif //FORMULA_ML_FIXEDNETWORKMIDLINE_H
