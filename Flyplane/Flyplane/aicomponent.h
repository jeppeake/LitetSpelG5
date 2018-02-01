#pragma once
#include "behaviour.h"
struct AIComponent {
	AIComponent(std::vector<Behaviour*> behaviours) : behaviours(behaviours){}
	~AIComponent() {
		for (int i = 0; i < behaviours.size(); i++) {
			delete behaviours.at(i);
		}
	}
	std::vector<Behaviour*> behaviours;
	bool is_targeted = false;
};