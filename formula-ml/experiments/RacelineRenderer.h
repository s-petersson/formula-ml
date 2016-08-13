#ifndef EXPERIMENTS_RACELINERENDERER
#define EXPERIMENTS_RACELINERENDERER

#include <memory>

#include <gl/glew.h>

#include <core/util/Camera.h>
#include <core/gfx/Renderer.h>
#include <sim/Simulator.h>
#include <sim/view/GridView.h>
#include <sim/view/TrackView.h>
#include <core/gfx/Curve.h>


class RacelineRenderer : public Renderer {
public:
    RacelineRenderer(Simulator* s);
    ~RacelineRenderer();

    void initialize()       override;
    void render()           override;
    void update(float dt)   override;
    void reset()            override;
private:
    Simulator* sim;
    std::unique_ptr<GridView> grid_view;
    std::unique_ptr<TrackView> track_view;
    std::unique_ptr<gfx::Curve> raceline;
    std::unique_ptr<Camera> global_cam;

    GLuint shader;
};
#endif