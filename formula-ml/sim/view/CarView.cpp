#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "CarView.h"
#include <core/util/Util.h>

using namespace glm;

CarView::CarView(CarModel *dataModel) {
    this->dataModel = dataModel;

    // Set up the car model and color it red.
    viewModel = dataModel->get_model();
    viewModel->create_vao();
    std::vector<float> color;
    for(int i = 0; i < viewModel->get_raw_vertices().size(); i += 4) {
        color.push_back(1.0f); // Red
        color.push_back(0.0f); // Green
        color.push_back(0.0f); // Blue
        color.push_back(1.0f); // Alpha
    }

    glBindVertexArray(viewModel->getVAO());

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), &color[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

}

CarView::~CarView() {

}

void CarView::setUniformLocations(GLuint shaderProgram, char* modelMatrixUniform) {
    viewModel->set_model_matrix_loc(glGetUniformLocation(shaderProgram, modelMatrixUniform));
}

void CarView::render() {
	
    glm::mat4 car_transform = glm::translate(glm::mat4(1.0f), dataModel->position);
    float a = glm::acos(glm::dot(glm::vec3(1, 0, 0), glm::normalize(dataModel->direction)));
    a = (dataModel->direction.y >= 0 ? a : -a);
    car_transform = glm::rotate(car_transform, a, glm::vec3(0, 0, 1));

    glUniformMatrix4fv(viewModel->get_model_matrix_loc(), 1, GL_FALSE, glm::value_ptr(car_transform));
    glBindVertexArray(viewModel->getVAO());
        glDrawElements(GL_TRIANGLES, viewModel->get_indices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}