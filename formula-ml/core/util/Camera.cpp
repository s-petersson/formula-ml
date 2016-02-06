#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <OpenGL/OpenGL.h>
#include <external/include/glm/gtc/type_ptr.hpp>


Camera::Camera(float fov, float aspect, float near, float far) {
    projectionMatrix = glm::perspective(fov, aspect, near, far);
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

void Camera::setUniformLocations (GLuint shaderProgram, char* viewMatrixName, char* projectionMatrixName) {
    viewUniformLoc = glGetUniformLocation(shaderProgram, viewMatrixName);
    projUniformLoc = glGetUniformLocation(shaderProgram, projectionMatrixName);
}

void Camera::update() {
    glUniformMatrix4fv(viewUniformLoc, 1, GL_FALSE, glm::value_ptr(getViewMatrix()));
    glUniformMatrix4fv(projUniformLoc, 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));
}