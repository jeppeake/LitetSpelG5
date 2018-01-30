#pragma once
#include <glm\glm.hpp>

struct Missile {
	Missile(glm::vec3 target) : target(target)  {}
	
	glm::vec3 target = glm::vec3(0,0,0);
};