#include "Car.h"
#include "core/Keyboard.h"
#include <iostream>
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>
const float mass = 642;				// [kg]
const float my = 0.8f;				// µ - friction coefficient. Guessed
const float g = 9.82f;
const float downforceConstant = mass * g / 36.1111 / 36.1111; // d*v^2 = mg at 130 km/h

const float gasForce = 14.2 * mass;		// [N]
const float brakeForce = -39 * mass;	// [N]
const float minTurningRadius = 4;		// Guessed


using namespace glm;

Car::Car() {
	position = vec3();
	direction = vec3(1, 0, 0);
	velocity = vec3();
}

void Car::update(float dt) {
	bool gas = isKeyDown(GLFW_KEY_UP);
	bool brake = isKeyDown(GLFW_KEY_DOWN);
	bool steerLeft = isKeyDown(GLFW_KEY_LEFT);
	bool steerRight = isKeyDown(GLFW_KEY_RIGHT);

	float currentSpeed = glm::length(velocity);

	// Acceleration in the direction of the car
	float forwardForce = 0;
	if (gas && !brake) {
		forwardForce = std::min(gasForce, maxTyreForce(currentSpeed));
		velocity += direction * (forwardForce * dt / mass);
	}
	else if (!gas && brake) {
		forwardForce = std::min(brakeForce, maxTyreForce(currentSpeed));
		velocity += direction * (forwardForce * dt / mass);
		if (length(normalize(velocity) + direction) == 0) {
			velocity *= 0;
		}
	}

	// Rotation due to steering
	if (steerLeft && !steerRight) {
		float rotation = maxRotation(currentSpeed, forwardForce, dt);
		direction = glm::rotateZ(direction, rotation);
		velocity = glm::rotateZ(velocity, rotation);
	}
	else if (!steerLeft && steerRight) {
		float rotation = -maxRotation(currentSpeed, forwardForce, dt);
		direction = glm::rotateZ(direction, rotation);
		velocity = glm::rotateZ(velocity, rotation);
	}

	// Apply velocity
	position += velocity * dt;
}

// Calculate the maximum rotation in radians for a delta time dt
float Car::maxRotation(float speed, float forwardForce, float dt) {
	float minradius = minRadius(speed, forwardForce);
	return speed * dt / minradius;
}

// Calculate the minimum the radius the car can handle at
// current "speed" and engine/brake "forwardForce"
float Car::minRadius(float speed, float forwardForce) {
	float tyreForce = maxTyreForce(speed);
	float centralForce = sqrt(tyreForce*tyreForce - forwardForce*forwardForce);
	float result = mass*speed*speed / centralForce;
	return std::max(result, minTurningRadius);
}

// Calculate the maximum force the tyres can apply to the road
// Assuming same grip in all directions
float Car::maxTyreForce(float speed) {
	return downforceConstant*my*speed*speed + mass*g*my;
}
