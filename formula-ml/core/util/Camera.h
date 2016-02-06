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
    void moveX(float amount);
    void moveY(float amount);

    void setUniformLocations(GLuint shaderProgram, char* viewMatrixName, char* projectionMatrixName);
    void update();

private:
    glm::mat4x4 projectionMatrix;
    glm::vec3 position;

    GLuint viewUniformLoc;
    GLuint projUniformLoc;
};

#endif