#include "SimulationState.h"

#include <GL/glew.h>
#include <core/gfx/Program.h>
#include <core/Keyboard.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/util/Util.h>

GLuint vertexArrayObject;
GLuint shader;
GLuint projectionMatrixUniform;
GLuint viewMatrixUniform;


struct Grid {
private:
    GLuint vao;
public:
    Grid() {
        glm::vec4 * positions = new glm::vec4[44];
        glm::vec4 * colours = new glm::vec4[44];

        for (int i = 0; i < 11; i++) {
            positions[2 * i] = glm::vec4(-10 + 2 * i, 10.0f, 0, 1.0f);
            positions[2 * i + 1] = glm::vec4(-10 + 2 * i, -10.0f, 0, 1.0f);
        }

        for (int i = 0; i < 11; i++) {
            positions[22 + 2 * i] = glm::vec4(-10.0f , -10 + 2 * i, 0, 1.0f);
            positions[22 + 2 * i + 1] = glm::vec4(10.0f, -10 + 2 * i, 0, 1.0f);
        }


        for (int i = 0; i < 44; i++) {
            colours[i] = glm::vec4(0.7f, 0.7f, 1.0f, 1.0f);
        }

        GLuint positionBuffer;
        glGenBuffers(1, &positionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 44, positions, GL_STATIC_DRAW);

        GLuint colorBuffer;
        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 44, colours, GL_STATIC_DRAW);
        
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);

        glEnableVertexAttribArray(0); 
        glEnableVertexAttribArray(1);
        delete[] positions;
        delete[] colours;
    }

    void render() {
        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, 44);
    }
};


SimulationState::SimulationState() {
	sim = new Simulator();
    camera = new Camera(90.0f, 16.0f/9, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0, 0, 3));

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

    camera->setUniformLocations(shader, "viewMatrix", "projectionMatrix");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    grid = new Grid;
}


SimulationState::~SimulationState() {
	if (sim) delete sim;
    if (grid) delete grid;
}

void SimulationState::update(float dt) {
	sim->update(dt);

    if (isKeyDown(GLFW_KEY_UP)) camera->moveY(dt * 5.0f); 
    if (isKeyDown(GLFW_KEY_DOWN)) camera->moveY(dt * -5.0f);

    if (isKeyDown(GLFW_KEY_RIGHT)) camera->moveX(dt * 5.0f);
    if (isKeyDown(GLFW_KEY_LEFT)) camera->moveX(dt * -5.0f);
}

void SimulationState::render() {
    glDisable(GL_CULL_FACE);
    glUseProgram(shader);

    camera->update();
    grid->render();

    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);
}
