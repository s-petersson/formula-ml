#ifndef CORE_UTIL_CAMERA
#define CORE_UTIL_CAMERA

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <external/include/GL/glew.h>

class Camera {
public:
    Camera(float fov, float aspect, float near, float far);
    ~Camera();

    glm::mat4x4 getProjectionMatrix();
    glm::mat4x4 getViewMatrix();
    void setPosition(glm::vec3 pos);
    void lookAt(glm::vec3 target);
    void follow(glm::vec3 pos, glm::vec3 target, float distance);
    void moveX(float amount);
    void moveY(float amount);
    void moveZ(float amount);

    void setUniformLocations(GLuint shaderProgram, char* viewMatrixName, char* projectionMatrixName);
    void update();

    glm::vec3 up = glm::vec3(0, 0, 1);
private:
    glm::mat4x4 projectionMatrix;
    glm::vec3 position;
    glm::vec3 target;

    GLuint viewUniformLoc;
    GLuint projUniformLoc;
};

#endif