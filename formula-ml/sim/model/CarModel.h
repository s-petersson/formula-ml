#ifndef SIM_MODEL_CAR
#define SIM_MODEL_CAR


#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <string>

class CarModel {
public:
    CarModel();
    ~CarModel();
    void update(float dt);

    glm::vec3 position;
    glm::vec3 direction;

private:
    float maxRotation(float speed, float forwardForce, float dt);
    float minRadius(float speed, float forwardForce);
    float maxTyreForce(float speed);
    glm::vec3 velocity;
};


#endif
