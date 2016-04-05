#include <sim/Simulator.h>
#include <core/gfx/Program.h>
#include "StandardRenderer.h"
#include <string>

StandardRenderer::StandardRenderer(Simulator* simulator) {
    this->simulator = simulator;
    initialize();
}

StandardRenderer::~StandardRenderer() {
    if (grid_view)  delete grid_view;
    if (camera)     delete camera;
    if (car_view)   delete car_view;
    if (track_view) delete track_view;
}

void StandardRenderer::initialize() {
    camera = new Camera(90.0f, 16.0f/9, 0.f, 1000.0f);

    // Create views for simulated objects.
    car_view = new CarView(this->simulator->car);
    
	track_view = new TrackView(this->simulator->track);

    // And one grid to keep one from loosing ones mind.
    grid_view = new GridView();

    // Create shaders to render stuff with. We only have one shader for now.
    shader = CreateShader("./res/shaders/simple.vert", "./res/shaders/simple.frag");

    // We set the appropriate uniform locations in all things that we
    // use to draw with.
    camera->setUniformLocations(shader, "viewMatrix", "projectionMatrix");
    car_view->setUniformLocations(shader, "modelMatrix");
    track_view->setUniformLocations(shader, "modelMatrix");
    grid_view->setUniformLocations(shader, "modelMatrix");
	gui = new gui::View();
	gui->add_text("Distance: " + std::to_string(0.0f), 32, glm::vec3(20, 720, 0), glm::vec4(1.0f, 0.33f, 0.67f, 1.0f));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void StandardRenderer::render() {
    glDisable(GL_CULL_FACE);

    // Draw with appropriate shader, we only have one
    // shader so we use it to draw all things.
    glUseProgram(shader);

    // Upload camera view and projection matrices to the shader.
    camera->follow(simulator->car->position, simulator->car->direction, 35.f);
    camera->update();

    // Upload model matrix for the grid to the shader and render the grid
    // using that.
    grid_view->render();

    // Render the model, this uploads a model matrix and renders using that.
    track_view->render();
    car_view->render();
	//gui->clear();
	//gui->add_text("Distance: " + std::to_string(simulator->result.distance_driven), 32, glm::vec3(20, 720, 0), glm::vec4(1.0f, 0.33f, 0.67f, 1.0f));
	gui->render();
}