#ifndef SIM_MODEL_CAR
#define SIM_MODEL_CAR


#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <string>
#include <core/gfx/Model.h>
#include "TrackModel.h"


struct CarControl {
	/*
	Acceleration percentage
	0 <= accelerate <= 1
	*/
	float acceleration;

	/*
	Brake percentage
	Subordinated acceleration
	0 <= brake <= 1
	*/
	float brake;

	/*
	Steering percentage
	Subordinated accelerate and brake
	-1 <= steer <= 1
	Anti-clockwise: -1 right, 0 neutral, 1 left
	*/
	float steer;
};


class CarModel {
public:
    CarModel();
    ~CarModel();
    void update(float dt, struct CarControl control);
	inline float getSpeed();
	Model* get_model();

    glm::vec3 position;
    glm::vec3 direction;

    int checkpoint;
    float distance_on_track;

private:
    float maxRotation(float speed, float forwardForce, float dt);
    float minRadius(float speed, float forwardForce);
    float maxTyreForce(float speed);
	float dragForce(float speed);

	glm::vec3 velocity;
	CarControl currentControl;
	Model* model;
};
#endif
