#include <sim/Simulator.h>
#include <core/gfx/Program.h>
#include "StandardRenderer.h"
#include <string>
#include <core/Keyboard.h>
#include <core/util/Util.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

StandardRenderer::StandardRenderer(Simulator* simulator) {
    this->simulator = simulator;
    camera_switch_time = 0;
    initialize();
}

StandardRenderer::~StandardRenderer() {
	if (simulator) delete simulator;
}

void StandardRenderer::initialize() {
    follow_cam = make_unique<Camera>(90.0f, 16.0f/9, 0.f, 1000.0f);
    global_cam = make_unique<Camera>(90.0f, 16.0f / 9, 0.f, 1000.0f);

    // Create views for simulated objects.
    car_view = make_unique<CarView>(simulator->car);
    
	track_view = make_unique<TrackView>(simulator->track);

    // And one grid to keep one from loosing ones mind.
    grid_view = make_unique<GridView>();
	car_trail = make_unique<gfx::Curve>();
	turn_radius = make_unique<gfx::Curve>();
    // Create shaders to render stuff with. We only have one shader for now.
    shader = CreateShader("./res/shaders/simple.vert", "./res/shaders/simple.frag");

    // We set the appropriate uniform locations in all things that we
    // use to draw with.
    follow_cam->setUniformLocations(shader, "viewMatrix", "projectionMatrix");
    follow_cam->up = vec3(1, 0, 0);
    
    global_cam->setUniformLocations(shader, "viewMatrix", "projectionMatrix");
    global_cam->up = vec3(1, 0, 0);


    car_view->setUniformLocations(shader, "modelMatrix");
    track_view->setUniformLocations(shader, "modelMatrix");
    grid_view->setUniformLocations(shader, "modelMatrix");
	gui = make_unique<gui::View>();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void StandardRenderer::render() {
	{ // Update the car trail
		vec4 pos = vec4(simulator->car->position,1);
		vec4 col = vec4(0, 0, 1, 1) + (vec4(1, 0, -1, 1) * (simulator->car->getSpeed() / 75.0f));
		car_trail->append_vertex(pos, col);
	}

	turn_radius->clear();
	{ // Create the turning radius curve
		auto radius = simulator->car->min_turning_radius();
		auto dir = simulator->car->direction;
		auto arc_center = vec4(simulator->car->position + normalize(vec3(dir.y, -dir.x, 0)) * radius, 1);
		auto vertex_dir = vec4(simulator->car->position, 1) - arc_center;
		auto rotmat = glm::rotate(mat4(1), 3.1415f / 20, vec3(0, 0, -1));
		
		for (int i = 0; i < 10; i++) {
			turn_radius->append_vertex(arc_center + vertex_dir, vec4(1, 1, 1, 1));
			vertex_dir = rotmat * vertex_dir;
		}
	}
	
	
	glDisable(GL_CULL_FACE);
    // Draw with appropriate shader, we only have one
    // shader so we use it to draw all things.
    glUseProgram(shader);
	
    // Upload camera view and projection matrices to the shader.
    //follow_cam->follow(simulator->car->position, simulator->car->direction, 35.f);
    {
        long time = util::current_time();
        if (isKeyDown(GLFW_KEY_SPACE) && time > (camera_switch_time + 100)) {
            follow = !follow;
            camera_switch_time = time;
        }
    }
    
    if (follow) {
		follow_cam->setPosition(simulator->car->position + vec3(0, 0, 64));
        follow_cam->lookAt(simulator->car->position);
        follow_cam->update();
    }
    else {
		global_cam->setPosition(vec3(300, 100, 256));
        global_cam->lookAt(vec3(global_cam->position.x,global_cam->position.y,0));
        global_cam->update();
    }
    // Upload model matrix for the grid to the shader and render the grid
    // using that.
    grid_view->render();

    // Render the model, this uploads a model matrix and renders using that.
    track_view->render();
	car_trail->render();
	turn_radius->render();
	car_view->render();
	
	gui->clear();
	gui->add_text("Distance: " + std::to_string(simulator->result.distance_on_track), 16, vec3(20, 720, 0), vec4(1.0f, 0.33f, 0.67f, 1.0f));
    gui->add_text("To left edge: " + std::to_string(simulator->distance_to_left_edge()), 16, vec3(20, 700, 0), vec4(1.0f, 0.33f, 0.67f, 1.0f));
    gui->add_text("To right edge: "+ std::to_string(simulator->distance_to_right_edge()), 16, vec3(20, 680, 0), vec4(1.0f, 0.33f, 0.67f, 1.0f));
	gui->render();    
}

void StandardRenderer::update(float dt) {

}


void StandardRenderer::reset() {
	car_trail->clear();
}