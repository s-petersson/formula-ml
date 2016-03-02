#ifndef FORMULA_ML_SIMULATIONRENDERER_H
#define FORMULA_ML_SIMULATIONRENDERER_H

#include <core/gfx/Renderer.h>
#include "Simulator.h"

class SimulationRenderer : public Renderer {
public:
    ~SimulationRenderer() {
        delete simulator;
    }

    virtual void initialize() = 0;
    virtual void render() = 0;
protected:
    Simulator * simulator;
};

#endif
