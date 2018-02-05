#pragma once
#include <glm\glm.hpp>
#include "targetcomponent.h"

struct Missile {
	Missile(Transform* target, float speed, float turnRate) : target(target), turnRate(turnRate), speed(speed)  {}
	
	Transform* target;
	float speed, turnRate;
};