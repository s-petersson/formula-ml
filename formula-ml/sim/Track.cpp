#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Track.h"

Track::Track() {
    model = new Model("./res/models/spa_circuit.model");

    // Per vertex color.
    float * color = new float[model->getVertices().size()];

    for(int i = 0; i < model->getVertices().size(); i += 4) {
        color[i] = 0.5f; // Red
        color[i + 1] = 0.5f; // Green
        color[i + 2] = 0.5f; // Blue
        color[i + 3] = 1.0f; // Alpha
    }

    glBindVertexArray(model->getVAO());

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    delete[] color;
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