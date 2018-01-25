#pragma once

#include <glm\glm.hpp>
struct Physics {
	const float density = 1.2466;
	const glm::vec3 g = glm::vec3(0.0, -9.82, 0.0);
	Physics(double mass, double dragCoeff, float turnRate, glm::vec3 velocity, glm::vec3 acceleration) : mass(mass), dragCoeff(dragCoeff), turnRate(turnRate), velocity(velocity), acceleration(acceleration){

	}
	float calcTotalDrag () { //move functions to system
		return 0.5 * density * dragCoeff * glm::length(velocity);
	}
	double mass, dragCoeff, turnRate;
	glm::vec3 acceleration, added_forces, velocity;
};