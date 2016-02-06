#ifndef CORE_UTIL_ORTHO_CAMERA
#define CORE_UTIL_ORTHO_CAMERA

#include <glm/mat4x4.hpp>

class OrthoCamera {
public:
    OrthoCamera(float near, float far, float x, float y);

    glm::mat4x4 getProjectionMatrix();
    glm::mat4x4 getViewMatrix();
    void setPosition(glm::vec2 pos);
    void moveX(float amount);
    void moveY(float amount);

private:
    glm::mat4x4 projectionMatrix;
    glm::mat4x4 viewMatrix;
};

#endif /* OrthoCamera_hpp */
