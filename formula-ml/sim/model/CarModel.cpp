#include "CarModel.h"
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

const float mass = 642;				// [kg]
const float my = 0.8f;				// µ - friction coefficient. Guessed
const float g = 9.82f;
const float downforceConstant = mass * g / 36.1111 / 36.1111; // c_down * v^2 = mg at 130 km/h
const float dragConstant = g / 83.3333 / 83.3333; // c_drag * v^2 = 1g at 300 km/h
const float gasForce = 14.2 * mass;		// [N]
const float brakeForce = 39 * mass;	    // [N]
const float minTurningRadius = 4;		// Guessed

using namespace glm;

CarModel::CarModel(glm::vec3 position, glm::vec3 direction, float max_speed) {
    model                   = new Model("./res/models/car.model");
    this->position          = position;
    this->initial_position  = position;
	this->measure_point		= position;

    this->direction         = direction;
    this->initial_direciton = direction;

    this->max_speed         = max_speed;
    this->initial_max_speed = max_speed;

    checkpoint              = 1;
    distance_on_track       = 0;
    velocity                = vec3();
	current_control         = CarControl();
}

CarModel::~CarModel() {

}

void CarModel::reset() {
    position                        = initial_position;
    direction                       = initial_direciton;
    max_speed                       = initial_max_speed;
    checkpoint                      = 1;
    distance_on_track               = 0;
    velocity                        = vec3();
    current_control.acceleration    = 0;
    current_control.brake           = 0;
    current_control.steer           = 0;
}

float CarModel::getSpeed() {
	return glm::length(velocity);
}
void CarModel::setSpeed(float speed) {
	velocity = speed * direction;
}

Model* CarModel::get_model() {
    return model;
}
void smoothChange(float* value, float new_value, float dt, float value_range) {
    const float max_change_time = 0.35f;
    const float max_change = dt * value_range / max_change_time;
    float change = new_value - *value;
    *value += change >= 0 ? glm::min(change, max_change) : glm::max(change, -max_change);
}

void CarModel::update(float dt, struct CarControl control) {

	// Update current control state with smoothing
    smoothChange(&current_control.acceleration, control.acceleration, dt, 1.f);
    smoothChange(&current_control.brake, control.brake, dt, 1.f);
    smoothChange(&current_control.steer, control.steer, dt, 2.f);

    float currentSpeed = getSpeed();

    // Acceleration in the direction of the car
    float forwardForce = 0;
    if (current_control.acceleration > 0 && currentSpeed < max_speed) {
        forwardForce = glm::min(gasForce, maxTyreForce(currentSpeed)) * current_control.acceleration;
        velocity += direction * (forwardForce * dt / mass);
    } else if (current_control.brake <= 0 && currentSpeed < max_speed) {
        forwardForce = glm::min(gasForce, maxTyreForce(currentSpeed)) * 1;
        velocity += direction * (forwardForce * dt / mass);
    } else if (current_control.brake > 0) {
        forwardForce = -glm::min(brakeForce, maxTyreForce(currentSpeed)) * current_control.brake;
        velocity += direction * (forwardForce * dt / mass);
        if (length(normalize(velocity) + direction) < 1) {
            velocity *= 0;
        }
    }

    // Rotation due to steering
    if (current_control.steer != 0) {
        float rotation = maxRotation(currentSpeed, forwardForce, dt) * current_control.steer;
        direction = glm::rotateZ(direction, rotation);
        velocity = glm::rotateZ(velocity, rotation);
    }

	// Apply drag
	//velocity -= velocity * (dragForce(currentSpeed) * dt / mass);

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
    return 1*(downforceConstant*my*speed*speed + mass*g*my);
}

float CarModel::dragForce(float speed) {
	return dragConstant * speed * speed;
}

