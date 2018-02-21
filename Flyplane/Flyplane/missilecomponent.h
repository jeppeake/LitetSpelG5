#pragma once
#include <glm\glm.hpp>
#include "targetcomponent.h"

struct Missile {
	Missile(Transform* target, float speed, float turnRate, float detonateRange, float explodeRadius, float explodeDamage, float droptime = 1) : target(target), turnRate(turnRate), speed(speed),
	detonateRange(detonateRange), explodeRadius(explodeRadius), explodeDamage(explodeDamage), droptime(droptime) {}
	
	Transform* target;
	float speed, turnRate, detonateRange, explodeRadius, explodeDamage, droptime;
	bool shouldExplode = false;
};