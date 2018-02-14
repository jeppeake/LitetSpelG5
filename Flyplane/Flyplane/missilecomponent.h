#pragma once
#include <glm\glm.hpp>
#include "targetcomponent.h"

struct Missile {
	Missile(Transform* target, float speed, float turnRate, float detonateRange, float explodeRadius, float explodeDamage) : target(target), turnRate(turnRate), speed(speed),
	detonateRange(detonateRange), explodeRadius(explodeRadius), explodeDamage(explodeDamage) {}
	
	Transform* target;
	float speed, turnRate, detonateRange, explodeRadius, explodeDamage;
	bool shouldExplode = false;
};