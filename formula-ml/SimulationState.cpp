#include "SimulationState.h"

#include <GL/glew.h>
#include <core/gfx/Program.h>
#include <core/Keyboard.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/util/Util.h>
#include <glm/gtc/matrix_transform.hpp>

GLuint vertexArrayObject;
GLuint shader;
GLuint projectionMatrixUniform;
GLuint viewMatrixUniform;
GLuint modelMatrixUniform;

struct Grid {
private:
    GLuint vao;
public:
    Grid() {
        glm::vec4 * positions = new glm::vec4[88];
        glm::vec4 * colours = new glm::vec4[88];

        for (int i = 0; i < 22; i++) {
            positions[2 * i] = glm::vec4(-1000 + 100 * i, 1000.0f, 0, 1.0f);
            positions[2 * i + 1] = glm::vec4(-1000 + 100 * i, -1000.0f, 0, 1.0f);
        }

        for (int i = 0; i < 22; i++) {
            positions[44 + 2 * i] = glm::vec4(-1000.0f , -1000 + 100 * i, 0, 1.0f);
            positions[44 + 2 * i + 1] = glm::vec4(1000.0f, -1000 + 100 * i, 0, 1.0f);
        }


        for (int i = 0; i < 88; i++) {
            colours[i] = glm::vec4(0.7f, 0.7f, 1.0f, 1.0f);
        }

        GLuint positionBuffer;
        glGenBuffers(1, &positionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 88, positions, GL_STATIC_DRAW);

        GLuint colorBuffer;
        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 88, colours, GL_STATIC_DRAW);
        
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
        glDrawArrays(GL_LINES, 0, 88);
    }
};


SimulationState::SimulationState() {
	sim = new Simulator();
    camera = new Camera(90.0f, 16.0f/9, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0, 0, 3));


    const float positions[] = {
        -2.0f, 1.0f, 1.0f, 1.0f,
        -2.0f, -1.0f, 1.0f, 1.0f,
        3.0f, 0.0f, 1.0f, 1.0f,
    };

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
    modelMatrixUniform = glGetUniformLocation(shader, "modelMatrix");

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
    glm::vec3 carpos = sim->car->position;
    carpos.z = 32;
    camera->setPosition(carpos);
}

void SimulationState::render() {
    glDisable(GL_CULL_FACE);
    glUseProgram(shader);

    camera->update();

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

    grid->render();

    glm::mat4 car_transform = glm::translate(glm::mat4(1.0f), sim->car->position);
    float a = glm::dot(glm::normalize(sim->car->velocity), glm::vec3(1, 0, 0));
    car_transform = glm::rotate(car_transform, glm::acos(a), glm::vec3(0, 0, 1));
    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(car_transform));
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);

}
