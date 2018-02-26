#pragma once

#include <glm\glm.hpp>
struct Physics {
public:
	const float density = 1.2466;
	const glm::vec3 g = glm::vec3(0.0, -9.82, 0.0);
	Physics(double mass, double dragCoeff, glm::vec3 velocity, glm::vec3 acceleration) : mass(mass), dragCoeff(dragCoeff), velocity(velocity), acceleration(acceleration){

	}
	float calcTotalDrag () {
		return 0.5 * density * dragCoeff * glm::length(velocity);
	}
	double mass, dragCoeff;
	glm::vec3 acceleration = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 velocity = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
	float drift = 0;
	bool gravity = true;
};