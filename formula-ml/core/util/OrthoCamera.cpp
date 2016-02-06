#include "OrthoCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


OrthoCamera::OrthoCamera (float near, float far, float x, float y) {
    projectionMatrix = glm::ortho(-10, 10, -10 ,10);
    viewMatrix = glm::lookAt(glm::vec3(x, y, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    //viewMatrix = glm::mat4(1);
}

glm::mat4x4 OrthoCamera::getProjectionMatrix() {
    return projectionMatrix;
}

glm::mat4x4 OrthoCamera::getViewMatrix() {
    return viewMatrix;
}

void OrthoCamera::setPosition(glm::vec2 pos) {
    glm::translate(viewMatrix, glm::vec3(pos.x, pos.y, 0));
}

void OrthoCamera::moveX(float amount) {
    glm::translate(viewMatrix, glm::vec3(amount, 0, 0));
}

void OrthoCamera::moveY(float amount) {
    glm::translate(viewMatrix, glm::vec3(0, amount, 0));
}