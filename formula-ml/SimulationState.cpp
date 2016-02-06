#include "SimulationState.h"

#include <GL/glew.h>
#include <core/gfx/Program.h>
#include <core/Keyboard.h>
#include <core/util/OrthoCamera.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/util/Util.h>


GLuint vertexArrayObject;
GLuint shader;
GLuint projectionMatrixUniform;
GLuint viewMatrixUniform;


SimulationState::SimulationState() {
	sim = new Simulator();
    camera = new OrthoCamera(0.f, 100.f, 0.f, 0.f);

    const float positions[] = {
        0.0f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 1.0f, 1.0f,
    };

    glm::vec3(1,1,1);

	// Define the colors for each of the three vertices of the triangle
	const float colors[] = {
		//  R     G		B
		1.0f, 0.0f, 0.0f,1.0f,		// White
		0.0f, 1.0f, 0.0f,1.0f,		// White
		0.0f, 0.0f, 1.0f,0.0f		// White
	};

	GLuint positionBuffer;
	glGenBuffers(1, &positionBuffer);

	// Set the newly created buffer as the current one
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	// Send the vertex position data to the current buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	// Create a handle for the vertex color buffer
	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	// Set the newly created buffer as the current one
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	// Send the vertex color data to the current buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArrayObject);

	// Bind the vertex array object
	// The following calls will affect this vertex array object.
	glBindVertexArray(vertexArrayObject);
	// Makes positionBuffer the current array buffer for subsequent calls.
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	// Attaches positionBuffer to vertexArrayObject, in the 0th attribute location
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);

	// Makes colorBuffer the current array buffer for subsequent calls.
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	// Attaches colorBuffer to vertexArrayObject, in the 1st attribute location
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);

	glEnableVertexAttribArray(0); // Enable the vertex position attribute
	glEnableVertexAttribArray(1); // Enable the vertex color attribute

	shader = CreateShader("./res/shaders/simple.vert", "./res/shaders/simple.frag");

    viewMatrixUniform = glGetUniformLocation(shader, "viewMatrix");
    projectionMatrixUniform = glGetUniformLocation(shader, "projectionMatrix");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}


SimulationState::~SimulationState() {
	if (sim) delete sim;
}

void SimulationState::update(float dt) {
	sim->update(dt);
}

void SimulationState::render() {

	if (!isKeyDown(GLFW_KEY_SPACE)) {
		glDisable(GL_CULL_FACE);
		glUseProgram(shader);
        glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
		glBindVertexArray(vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(0);

	}
    

}
