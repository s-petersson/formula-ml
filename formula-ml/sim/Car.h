#ifndef SIM_CAR
#define SIM_CAR
#include <glm/glm.hpp>

class Car {
public:
	Car();
	void update(float dt);

	glm::vec3 position;
	glm::vec3 direction;

private:
	glm::vec3 velocity;
};

#endif