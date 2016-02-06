#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

Camera::Camera(float fov, float aspect, float near, float far, glm::vec3 pos) {
    projectionMatrix = glm::perspective(fov, aspect, near, far);
    position = pos;
}

Camera::~Camera() {}

glm::mat4x4 Camera::getProjectionMatrix() {
    return projectionMatrix;
}

glm::mat4x4 Camera::getViewMatrix() {
    return glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Camera::setPosition(glm::vec3 pos) {
    position = pos;
}

void Camera::moveX(float amount) {
    position.x += amount;
}

void Camera::moveY(float amount) {
    position.y += amount;
}