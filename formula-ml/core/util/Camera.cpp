#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <external/include/glm/gtc/type_ptr.hpp>
#include <ostream>
#include <iostream>


Camera::Camera(float fov, float aspect, float near, float far) {
    projectionMatrix = glm::perspective(fov, aspect, near, far);
}

Camera::~Camera() {}

glm::mat4x4 Camera::getProjectionMatrix() {
    return projectionMatrix;
}

glm::mat4x4 Camera::getViewMatrix() {
    return glm::lookAt(position, target, glm::vec3(0, 0, 1));
}

void Camera::setPosition(glm::vec3 pos) {
    position = pos;
}

void Camera::lookAt(glm::vec3 target) {
    this->target = target;
}

void Camera::follow(glm::vec3 targetPos, glm::vec3 targetDirection, float distance) {
    float angle = glm::acos(glm::dot(glm::vec3(0, 1, 0), targetDirection) / glm::length(targetDirection));
    if(glm::cross(glm::vec3(0, 1, 0), targetDirection).z > 0)
        angle = -angle;

    float horizDistance = distance * glm::cos(glm::radians(45.f));
    float vertiDistance = distance * glm::sin(glm::radians(45.f));

    float newX = targetPos.x - horizDistance * glm::sin(angle);
    float newY = targetPos.y - horizDistance * glm::cos(angle);
    float newZ = targetPos.z + vertiDistance;
    lookAt(targetPos);
    setPosition(glm::vec3(newX, newY, newZ));
}

void Camera::moveX(float amount) {
    position.x += amount;
}

void Camera::moveY(float amount) {
    position.y += amount;
}

void Camera::moveZ(float amount) {
    position.z += amount;
}

void Camera::setUniformLocations (GLuint shaderProgram, char* viewMatrixName, char* projectionMatrixName) {
    viewUniformLoc = glGetUniformLocation(shaderProgram, viewMatrixName);
    projUniformLoc = glGetUniformLocation(shaderProgram, projectionMatrixName);
}

void Camera::update() {
    glUniformMatrix4fv(viewUniformLoc, 1, GL_FALSE, glm::value_ptr(getViewMatrix()));
    glUniformMatrix4fv(projUniformLoc, 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));
}