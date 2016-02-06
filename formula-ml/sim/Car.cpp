#include "Car.h"
#include "core/Keyboard.h"
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>
const float rotationalSpeed = 1;
const float gasAcc = 20;
const float brakeAcc = -30;

Car::Car() {
	position = glm::vec3();
	velocity = glm::vec3();
}

void Car::update(float dt) {
	bool gas = isKeyDown(VK_UP);
	bool brake = isKeyDown(VK_DOWN);
	bool steerLeft = isKeyDown(VK_LEFT);
	bool steerRight = isKeyDown(VK_RIGHT);

	if (gas) {
		glm::vec3 deltaX = glm::vec3(dt * gasAcc, 0, 0);
		velocity += deltaX;
		std::cout << position.x << '\n';
	}
	else if (brake) {
		velocity += glm::vec3(dt * brakeAcc, 0, 0);
		std::cout << position.x << '\n';
	}

	if (steerLeft && !steerRight) {
		velocity = glm::rotateZ(velocity, rotationalSpeed * dt);
	}
	else if (!steerLeft && steerRight) {
        velocity = glm::rotateZ(velocity, -rotationalSpeed * dt);
	}

	position += velocity * dt;
}
