#ifndef FORMULA_ML_MANUALCONTROLRENDERER_H
#define FORMULA_ML_MANUALCONTROLRENDERER_H

#include <core/util/Camera.h>
#include <core/gfx/Renderer.h>
#include <core/gfx/Curve.h>
#include <core/gui/View.h>
#include <sim/view/TrackView.h>
#include <sim/view/CarView.h>
#include <sim/view/GridView.h>
#include <sim/Simulator.h>
#include <memory>

class StandardRenderer : public Renderer {
public:
    StandardRenderer(Simulator * simulator);
    ~StandardRenderer();

    virtual void initialize();
    virtual void render();
    virtual void update(float dt);
	virtual void reset();
private:
	Simulator * simulator;
	std::unique_ptr<Camera> global_cam;
	std::unique_ptr<Camera> follow_cam;
	
	std::unique_ptr<GridView> grid_view;
	std::unique_ptr<TrackView> track_view;
	std::unique_ptr<CarView> car_view;
	std::unique_ptr<gui::View> gui;
	std::unique_ptr<gfx::Curve> car_trail;
    GLuint shader;
    bool follow;
    long camera_switch_time;
};


#endif
