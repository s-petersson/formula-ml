#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sim/model/TrackModel.h>
#include "TrackView.h"

#include <core/util/Util.h>

using namespace glm;
using namespace std;

TrackView::TrackView(TrackModel *dataModel) {
	checkpoints_vao = 0;
	checkpoint_triangles = 0;
	this->dataModel = dataModel;

    viewModel = dataModel->get_model();
    
	viewModel->create_vao();
    
	// Per vertex color.
    std::vector<float> color;
    for(int i = 0; i < viewModel->get_raw_vertices().size(); i += 4) {
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



    vector<vec4> positions, colors;
    vector<Checkpoint> checkpoints = dataModel->get_checkpoints();
    for (auto && c : checkpoints) {
        positions.push_back(vec4(c.left, 1));
        colors.push_back(vec4(0, 1, 0, 1));

        positions.push_back(vec4(c.right, 1));
        colors.push_back(vec4(0, 1, 0, 1));
    }
    checkpoint_triangles = positions.size();
    glGenVertexArrays(1, &checkpoints_vao);
    glBindVertexArray(checkpoints_vao);

    GLuint posbuf, colbuf;
    glGenBuffers(1, &posbuf);
    glBindBuffer(GL_ARRAY_BUFFER, posbuf);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec4), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &colbuf);
    glBindBuffer(GL_ARRAY_BUFFER, colbuf);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec4), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

TrackView::~TrackView() {

}

void TrackView::setUniformLocations(GLuint shaderProgram, char* modelMatrixUniform) {
    viewModel->set_model_matrix_loc(glGetUniformLocation(shaderProgram, modelMatrixUniform));
}

void TrackView::render() {
	glUniformMatrix4fv(viewModel->get_model_matrix_loc(), 1, GL_FALSE, glm::value_ptr(viewModel->get_model_matrix()));
    glBindVertexArray(viewModel->getVAO());
    glDrawElements(GL_TRIANGLES, viewModel->get_indices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindVertexArray(checkpoints_vao);
    glDrawArrays(GL_LINES,0, checkpoint_triangles);
    glBindVertexArray(0);
}