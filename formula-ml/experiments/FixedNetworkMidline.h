#ifndef FORMULA_ML_FIXEDNETWORKMIDLINE_H
#define FORMULA_ML_FIXEDNETWORKMIDLINE_H


#include <core/Window.h>
#include <SimulationState.h>
#include <neural/FixedNetworkTrainer.h>
#include "Experiment.h"

class FixedNetworkMidline : public Experiment {
public:
    FixedNetworkMidline();
    ~FixedNetworkMidline();

    virtual void run();
    virtual std::function<CarControl()> updater();

private:
    SimulationState* simulationState;
    Window* window;
    neural::FixedNetworkTrainer trainer;
    neural::NetworkIO network_indata;
    neural::NetworkIO network_output;
};


#endif //FORMULA_ML_FIXEDNETWORKMIDLINE_H
