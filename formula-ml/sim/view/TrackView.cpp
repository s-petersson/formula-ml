#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sim/model/TrackModel.h>
#include "TrackView.h"

TrackView::TrackView(TrackModel *dataModel, Model * track_model) {
    this->dataModel = dataModel;

    viewModel = track_model;
    // Per vertex color.
    std::vector<float> color;
    for(int i = 0; i < viewModel->getVertices().size(); i += 4) {
        color.push_back(0.5f); // Red
        color.push_back(0.5f); // Green
        color.push_back(0.5f); // Blue
        color.push_back(1.0f); // Alpha
    }

    glBindVertexArray(viewModel->getVAO());

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), &color[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

TrackView::~TrackView() {
    // TODO: Clear OpenGL memory (colorBuffer)
    delete viewModel;
}

void TrackView::setUniformLocations(GLuint shaderProgram, char* modelMatrixUniform) {
    viewModel->setModelMatrixLoc(glGetUniformLocation(shaderProgram, modelMatrixUniform));
}

// TODO: Get modelMatrixUniformLoc from Model class.
void TrackView::render() {
    glUniformMatrix4fv(viewModel->getModelMatrixLoc(), 1, GL_FALSE, glm::value_ptr(viewModel->getModelMatrix()));
    glBindVertexArray(viewModel->getVAO());
    glDrawElements(GL_TRIANGLES, viewModel->getIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}