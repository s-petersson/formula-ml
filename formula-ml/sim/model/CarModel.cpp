#include "CarModel.h"
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

const float mass = 642;				// [kg]
const float my = 0.8f;				// � - friction coefficient. Guessed
const float g = 9.82f;
const float downforceConstant = mass * g / 36.1111 / 36.1111; // c_down * v^2 = mg at 130 km/h
const float dragConstant = g / 83.3333 / 83.3333; // c_drag * v^2 = 1g at 300 km/h
const float gasForce = 14.2 * mass;		// [N]
const float brakeForce = 39 * mass;	// [N]
const float minTurningRadius = 4;		// Guessed

using namespace glm;

CarModel::CarModel() {
    model = new Model("./res/models/car.model");
    position = vec3();
    direction = vec3(-1, 0, 0);
    velocity = vec3();
	currentControl = CarControl();
    checkpoint = 0;
    distance_on_track = 0;
}

CarModel::~CarModel() {

}
float inline CarModel::getSpeed() {
	return glm::length(velocity);
}

Model* CarModel::get_model() {
    return model;
}

void CarModel::update(float dt, struct CarControl control) {

	// Update current control state without delay
	// TODO: Check for illegal values
	currentControl = control;

    float currentSpeed = getSpeed();

    // Acceleration in the direction of the car
    float forwardForce = 0;
    if (currentControl.acceleration > 0) {
        forwardForce = glm::min(gasForce, maxTyreForce(currentSpeed)) * currentControl.acceleration;
        velocity += direction * (forwardForce * dt / mass);
    } else if (currentControl.brake > 0) {
        forwardForce = -glm::min(brakeForce, maxTyreForce(currentSpeed)) * currentControl.brake;
        velocity += direction * (forwardForce * dt / mass);
        if (length(normalize(velocity) + direction) < 1) {
            velocity *= 0;
        }
    }

    // Rotation due to steering
    if (currentControl.steer != 0) {
        float rotation = maxRotation(currentSpeed, forwardForce, dt) * currentControl.steer;
        direction = glm::rotateZ(direction, rotation);
        velocity = glm::rotateZ(velocity, rotation);
    }

	// Apply drag
	velocity -= velocity * (dragForce(currentSpeed) * dt / mass);

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
    return glm::max(result, minTurningRadius);
}

// Calculate the maximum force the tyres can apply to the road
// Assuming same grip in all directions
float CarModel::maxTyreForce(float speed) {
    return 2*(downforceConstant*my*speed*speed + mass*g*my);
}

float CarModel::dragForce(float speed) {
	return dragConstant * speed * speed;
}
