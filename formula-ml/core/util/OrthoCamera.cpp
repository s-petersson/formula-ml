#include "OrthoCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>
#include <iostream>

OrthoCamera::OrthoCamera (float near, float far, float x, float y) {
    projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near, far);
    lookAtMatrix = glm::lookAt(glm::vec3(x, y, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4x4 OrthoCamera::getProjectionMatrix() {
    return projectionMatrix;
}

glm::mat4x4 OrthoCamera::getViewMatrix() {
    return lookAtMatrix;
}

void OrthoCamera::setPosition(glm::vec2 pos) {
    glm::translate(lookAtMatrix, glm::vec3(pos.x, pos.y, 0));
}

void OrthoCamera::moveX(float amount) {
    glm::translate(lookAtMatrix, glm::vec3(amount, 0, 0));
}

void OrthoCamera::moveY(float amount) {
    glm::translate(lookAtMatrix, glm::vec3(0, amount, 0));
}