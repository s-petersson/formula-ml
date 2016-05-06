#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "CarView.h"
#include <core/util/Util.h>

using namespace glm;

CarView::CarView(CarModel *dataModel) {
    this->dataModel = dataModel;
    viewModel = dataModel->get_model();
    initCar();
}

CarView::~CarView() {

}

void CarView::initCar () {
    vector<float> vertices = viewModel->get_raw_vertices();
    vector<int> indices = viewModel->get_indices();
    vector<float> color;
    for(int i = 0; i < vertices.size(); i += 4) {
        color.push_back(1.0f); // Red
        color.push_back(0.0f); // Green
        color.push_back(0.0f); // Blue
        color.push_back(1.0f); // Alpha
    }

    glGenVertexArrays(1, &carVao);
    glBindVertexArray(carVao);

    // Load the vertices into a buffer
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    // Load the indices into a buffer
    GLuint indices_buffer;
    glGenBuffers(1, &indices_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // Enable the vertices attribute.
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);


    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), &color[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void CarView::setUniformLocations(GLuint shaderProgram, char* modelMatrixUniform) {
    modelMatrixLoc = glGetUniformLocation(shaderProgram, modelMatrixUniform);
}

void CarView::render() {
	
    glm::mat4 car_transform = glm::translate(glm::mat4(1.0f), dataModel->position);
    float a = glm::acos(glm::dot(glm::vec3(1, 0, 0), glm::normalize(dataModel->direction)));
    a = (dataModel->direction.y >= 0 ? a : -a);
    car_transform = glm::rotate(car_transform, a, glm::vec3(0, 0, 1));

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(car_transform));
    glBindVertexArray(carVao);
        glDrawElements(GL_TRIANGLES, (GLsizei) viewModel->get_indices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}