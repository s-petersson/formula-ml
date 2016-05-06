#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sim/model/TrackModel.h>
#include "TrackView.h"

#include <core/util/Util.h>

using namespace glm;
using namespace std;

TrackView::TrackView(TrackModel *dataModel) {
	checkpoints_vao = 0;
	checkpoint_triangle_count = 0;
	this->dataModel = dataModel;

    viewModel = dataModel->get_model();

    initTrack();
    initCheckpoints();
}

TrackView::~TrackView() {

}

void TrackView::initTrack() {
    vector<float> vertices = viewModel->get_raw_vertices();
    vector<int> indices = viewModel->get_indices();
    vector<float> color;
    for(int i = 0; i < vertices.size(); i += 4) {
        color.push_back(0.5f); // Red
        color.push_back(0.5f); // Green
        color.push_back(0.5f); // Blue
        color.push_back(1.0f); // Alpha
    }

    glGenVertexArrays(1, &track_vao);
    glBindVertexArray(track_vao);

    // Load the vertices into a buffer
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // Enable the vertices attribute.
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);


    // Load the indices into a buffer
    GLuint indices_buffer;
    glGenBuffers(1, &indices_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), &color[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TrackView::initCheckpoints() {
    vector<vec4> positions, colors;
    vector<Checkpoint> checkpoints = dataModel->get_checkpoints();
    for (auto && c : checkpoints) {
        positions.push_back(vec4(c.left, 1));
        colors.push_back(vec4(0, 1, 0, 1));

        positions.push_back(vec4(c.right, 1));
        colors.push_back(vec4(0, 1, 0, 1));
    }
    checkpoint_triangle_count = positions.size();

    glGenVertexArrays(1, &checkpoints_vao);
    glBindVertexArray(checkpoints_vao);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec4), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec4), &colors[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TrackView::setUniformLocations(GLuint shaderProgram, char* modelMatrixUniform) {
    modelMatrixLoc = glGetUniformLocation(shaderProgram, modelMatrixUniform);
}

void TrackView::render() {
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewModel->get_model_matrix()));
    glBindVertexArray(track_vao);
    glDrawElements(GL_TRIANGLES, viewModel->get_indices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindVertexArray(checkpoints_vao);
    glDrawArrays(GL_LINES,0, checkpoint_triangle_count);
    glBindVertexArray(0);
}