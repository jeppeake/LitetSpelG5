#pragma once

#include <glm\glm.hpp>
struct Physics {
	const float density = 1.2466;
	Physics(double mass, double drag, glm::vec3 velocity) : mass(mass), drag(drag), velocity(velocity) {

	}
	float calcDrag () {
		return 0.5 * drag * glm::length(velocity);
	}
	double mass, drag;
	glm::vec3 velocity;
};