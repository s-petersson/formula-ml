#ifndef CORE_UTIL_CAMERA
#define CORE_UTIL_CAMERA
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
    Camera(float fov, float aspect, float near, float far, glm::vec3 pos);
    ~Camera();

    glm::mat4x4 getProjectionMatrix();
    glm::mat4x4 getViewMatrix();
    void setPosition(glm::vec3 pos);
    void moveX(float amount);
    void moveY(float amount);

private:
    
    glm::mat4x4 projectionMatrix;
    glm::vec3 position;
};

#endif