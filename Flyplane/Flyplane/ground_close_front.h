#pragma once
#include "condition.h"
class Ground_Close_Front : public Condition {
public:
	Ground_Close_Front(float distance) : Condition() {
		this->distance = distance;
	}
	bool test(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) {

		return false;
	}
	float distance;
};