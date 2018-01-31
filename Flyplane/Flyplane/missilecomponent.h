#pragma once
#include <glm\glm.hpp>

struct Missile {
	Missile(Transform* target) : target(target)  {}
	
	Transform* target;
};