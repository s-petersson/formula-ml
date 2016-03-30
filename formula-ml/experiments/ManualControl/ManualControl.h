#ifndef FORMULA_ML_MANUALCONTROL_H
#define FORMULA_ML_MANUALCONTROL_H

#include <core/Window.h>
#include <core/Keyboard.h>
#include <experiments/Experiment.h>
#include <sim/SimulationState.h>

class ManualControl : public Experiment {
public:
    ManualControl();
    ~ManualControl();

    virtual void run();
    std::function<CarControl()> updater();

private:
    SimulationState* simulationState;
    Window* window;
    TrackGrid* grid;
};


#endif
