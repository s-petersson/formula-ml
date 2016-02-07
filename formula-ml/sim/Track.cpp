#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Track.h"

Track::Track() {
    model = new Model("./res/models/spa_circuit.model");

    // Per vertex color.
	std::vector<float> color;
    for(int i = 0; i < model->getVertices().size(); i += 4) {
		color.push_back(0.5f); // Red
		color.push_back(0.5f); // Green
		color.push_back(0.5f); // Blue
		color.push_back(1.0f); // Alpha
    }

    glBindVertexArray(model->getVAO());

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, color.size(), &color[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    // delete[] color;
}

Track::~Track() {
    // TODO: Clear OpenGL memory (colorBuffer)
    delete model;
}

// TODO: Get modelMatrixUniformLoc from Model class.
void Track::render(GLuint modelMatrixUniformLoc) {
    glUniformMatrix4fv(modelMatrixUniformLoc, 1, GL_FALSE, glm::value_ptr(model->getModelMatrix()));
    glBindVertexArray(model->getVAO());
        glDrawElements(GL_TRIANGLES, model->getIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}