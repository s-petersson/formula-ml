#ifndef FORMULA_ML_MANUALCONTROLRENDERER_H
#define FORMULA_ML_MANUALCONTROLRENDERER_H

#include <core/util/Camera.h>
#include <sim/view/TrackView.h>
#include <sim/view/CarView.h>
#include <sim/view/GridView.h>
#include <sim/SimulationRenderer.h>

class StandardRenderer : public SimulationRenderer {
public:
    StandardRenderer(Simulator * simulator);
    ~StandardRenderer();

    virtual void initialize();
    virtual void render();
private:
    Camera * camera;
    GridView * grid_view;
    TrackView * track_view;
    CarView * car_view;

    GLuint shader;
};


#endif
