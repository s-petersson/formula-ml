#include "SimulationState.h"
#include <core/gfx/Program.h>
#include <functional>

GLuint shader;

SimulationState::SimulationState(Simulator* simulator) {
    camera = new Camera(90.0f, 16.0f/9, 0.f, 1000.0f);

    this->sim = simulator;
    // Create views for simulated objects.
    carView = new CarView(this->sim->car);
    trackView = new TrackView(this->sim->track);

    // And one grid to keep one from loosing ones mind.
    gridView = new GridView();

    // Create shaders to render stuff with. We only have one shader for now.
	shader = CreateShader("./res/shaders/simple.vert", "./res/shaders/simple.frag");

    // We set the appropriate uniform locations in all things that we
    // use to draw with.
    camera->setUniformLocations(shader, "viewMatrix", "projectionMatrix");
    carView->setUniformLocations(shader, "modelMatrix");
    trackView->setUniformLocations(shader, "modelMatrix");
    gridView->setUniformLocations(shader, "modelMatrix");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}


SimulationState::~SimulationState() {
	if (sim) delete sim;
    if (gridView) delete gridView;
    if (camera) delete camera;
    if (carView) delete carView;
    if (trackView) delete trackView;
}

//Function declaration
void SimulationState::update(float dt) {
    sim->update(dt);

    // TODO: If "followCar"
    camera->follow(sim->car->position, sim->car->direction, 35.f);
}

void SimulationState::render() {
    glDisable(GL_CULL_FACE);

    // Draw with appropriate shader, we only have one
    // shader so we use it to draw all things.
    glUseProgram(shader);

    // Upload camera view and projection matrices to the shader.
    camera->update();

    // TODO: Do this in grid render method.
    // Upload model matrix for the grid to the shader and render the grid
    // using that.
    gridView->render();

    // Render the model, this uploads a model matrix and renders using that.
    trackView->render();
    carView->render();
}
