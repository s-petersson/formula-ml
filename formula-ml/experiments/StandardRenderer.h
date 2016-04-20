#ifndef FORMULA_ML_MANUALCONTROLRENDERER_H
#define FORMULA_ML_MANUALCONTROLRENDERER_H

#include <core/util/Camera.h>
#include <core/gfx/Renderer.h>
#include <core/gui/View.h>
#include <sim/view/TrackView.h>
#include <sim/view/CarView.h>
#include <sim/view/GridView.h>
#include <sim/Simulator.h>


class StandardRenderer : public Renderer {
public:
    StandardRenderer(Simulator * simulator);
    ~StandardRenderer();

    virtual void initialize();
    virtual void render();
private:
	Simulator * simulator;
    Camera * global_cam;
    Camera * follow_cam;

    GridView * grid_view;
    TrackView * track_view;
    CarView * car_view;
	gui::View * gui;
    GLuint shader;
    bool follow;
    long camera_switch_time;
};


#endif
