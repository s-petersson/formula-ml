#include "Car.h"
#include "core/Keyboard.h"
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>
const float rotationalSpeed = 1;
const float gasAcc = 20;
const float brakeAcc = -30;
const float drag_const = -5.0f;
const float roll_resistance = -100;
const float mass = 700;
const float gravity = 9.82;
const float weight = mass * gravity;


using namespace glm;

Car::Car() {
	position = vec3();
	direction = vec3(1, 0, 0);
	velocity = vec3();
}

void Car::update(float dt) {
	
	float engine_force = 0;
	if (isKeyDown(GLFW_KEY_UP)) {
		engine_force = 20000.0f;
	}
	else if (isKeyDown(GLFW_KEY_DOWN)) {
		engine_force = -10000.0f;
	}

    if (isKeyDown(GLFW_KEY_LEFT) && !isKeyDown(GLFW_KEY_RIGHT)) {
        direction = rotateZ(direction, rotationalSpeed * dt);
    }
    if (!isKeyDown(GLFW_KEY_LEFT) && isKeyDown(GLFW_KEY_RIGHT)) {
        direction = rotateZ(direction, -rotationalSpeed * dt);
        
    }
   

	vec3 traction = direction * engine_force;
	vec3 drag = drag_const * velocity * length(velocity);
	vec3 roll = roll_resistance * velocity;
	vec3 force = traction + drag + roll;
    
	vec3 a = force / mass;
    
	velocity += a * dt;
    position += velocity * dt;
}
