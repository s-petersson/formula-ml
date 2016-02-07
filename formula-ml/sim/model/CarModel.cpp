#include "CarModel.h"
#include "core/Keyboard.h"
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

const float mass = 642;				// [kg]
const float my = 0.8f;				// � - friction coefficient. Guessed
const float g = 9.82f;
const float downforceConstant = mass * g / 36.1111 / 36.1111; // d*v^2 = mg at 130 km/h
const float gasForce = 14.2 * mass;		// [N]
const float brakeForce = 39 * mass;	// [N]
const float minTurningRadius = 4;		// Guessed

using namespace glm;

CarModel::CarModel() {
    position = vec3();
    direction = vec3(1, 0, 0);
    velocity = vec3();
}

CarModel::~CarModel() {

}

void CarModel::update(float dt) {
    bool gas = isKeyDown(GLFW_KEY_UP);
    bool brake = isKeyDown(GLFW_KEY_DOWN);
    bool steerLeft = isKeyDown(GLFW_KEY_LEFT);
    bool steerRight = isKeyDown(GLFW_KEY_RIGHT);
    bool steerCareful = isKeyDown(GLFW_KEY_RIGHT_CONTROL);
    bool accelerateMax = isKeyDown(GLFW_KEY_RIGHT_SHIFT);

    float currentSpeed = glm::length(velocity);

    // Acceleration in the direction of the car
    float forwardForce = 0;
    if (gas && !brake) {
        forwardForce = std::min(gasForce, maxTyreForce(currentSpeed)) * (accelerateMax ? 1 : 0.5) * (steerLeft || steerRight ? 0.7 : 1);
        velocity += direction * (forwardForce * dt / mass);
    }
    else if (!gas && brake) {
        forwardForce = -std::min(brakeForce, maxTyreForce(currentSpeed)) * (accelerateMax ? 1 : 0.5) * (steerLeft || steerRight ? 0.7 : 1);
        velocity += direction * (forwardForce * dt / mass);
        if (length(normalize(velocity) + direction) < 1) {
            velocity *= 0;
        }
    }

    // Rotation due to steering
    if (steerLeft && !steerRight) {
        float rotation = maxRotation(currentSpeed, forwardForce, dt) * (steerCareful ? 0.5 : 1);
        direction = glm::rotateZ(direction, rotation);
        velocity = glm::rotateZ(velocity, rotation);
    }
    else if (!steerLeft && steerRight) {
        float rotation = -maxRotation(currentSpeed, forwardForce, dt) * (steerCareful ? 0.5 : 1);
        direction = glm::rotateZ(direction, rotation);
        velocity = glm::rotateZ(velocity, rotation);
    }

    // Apply velocity
    position += velocity * dt;
}

// Calculate the maximum rotation in radians for a delta time dt
float CarModel::maxRotation(float speed, float forwardForce, float dt) {
    float minradius = minRadius(speed, forwardForce);
    return speed * dt / minradius;
}

// Calculate the minimum the radius the car can handle at
// current "speed" and engine/brake "forwardForce"
float CarModel::minRadius(float speed, float forwardForce) {
    float tyreForce = maxTyreForce(speed);

    if (std::abs(tyreForce - std::abs(forwardForce)) < 0.01) {
        return INFINITY;
    }

    float centralForce = sqrt(tyreForce*tyreForce - forwardForce*forwardForce);
    float result = mass*speed*speed / centralForce;
    return std::max(result, minTurningRadius);
}

// Calculate the maximum force the tyres can apply to the road
// Assuming same grip in all directions
float CarModel::maxTyreForce(float speed) {
    return downforceConstant*my*speed*speed + mass*g*my;
}
