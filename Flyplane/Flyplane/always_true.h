#pragma once
#include "condition.h"
class Always_True : public Condition {
public:
	Always_True() : Condition() {

	}
	bool test(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest) {
		//std::cout << "Testing!\n";
		return true;
	}
};