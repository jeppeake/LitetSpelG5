#pragma once
#include <glm\glm.hpp>

struct Missile {
	Missile(glm::vec3 target) : target(target)  {}
	
	glm::vec3 target;
};