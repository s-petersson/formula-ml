#include "RacelineRenderer.h"
#include <glm/glm.hpp>
#include <core/gfx/Program.h>
#include <core/util/Util.h>
using namespace glm;

RacelineRenderer::RacelineRenderer(Simulator* s) {
    sim = s;
    initialize();
}


RacelineRenderer::~RacelineRenderer() {
    glUseProgram(0);
    glDeleteProgram(shader);
}


void RacelineRenderer::initialize() {
    shader = CreateShader("./res/shaders/simple.vert", "./res/shaders/simple.frag");
    raceline = make_unique<gfx::Curve>();
    
    grid_view = make_unique<GridView>();
    grid_view->setUniformLocations(shader, "modelMatrix");

    track_view = make_unique<TrackView>(sim->track);
    track_view->setUniformLocations(shader, "modelMatrix");
    
    global_cam = make_unique<Camera>(90.0f, 16.0f / 9, 0.f, 1000.0f);
    global_cam->setUniformLocations(shader, "viewMatrix", "projectionMatrix");
    global_cam->up = vec3(1, 0, 0);
}

void RacelineRenderer::render() {
    glUseProgram(shader);
    
    global_cam->setPosition(vec3(330, 100, 240));
    global_cam->lookAt(vec3(global_cam->position.x, global_cam->position.y, 0));
    global_cam->update();
    
    grid_view->render();
    track_view->render();
    raceline->render();
    glUseProgram(0);
}

void RacelineRenderer::update(float dt) {
    { // Update the car trail
        vec4 pos = vec4(sim->car->position, 1);
        vec4 col = vec4(0, 0, 1, 1) + (vec4(1, 0, -1, 1) * (sim->car->getSpeed() / 75.0f));
        raceline->append_vertex(pos, col);
    }
}

void RacelineRenderer::reset() {
    raceline->clear();
}