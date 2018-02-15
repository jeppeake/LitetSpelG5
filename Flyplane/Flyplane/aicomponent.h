#pragma once
#include "behaviour.h"
struct AIComponent {
	AIComponent(std::vector<Behaviour*> behaviours, bool drift_allowed, bool throttle_allowed, bool brake_allowed) : behaviours(behaviours), drift_allowed(drift_allowed), throttle_allowed(throttle_allowed), brake_allowed(brake_allowed){
	}
	~AIComponent() {
		for (int i = 0; i < behaviours.size(); i++) {
			delete behaviours.at(i);
		}
	}
	std::vector<Behaviour*> behaviours;
	bool is_targeted = false;
	bool drift_allowed = false;
	bool throttle_allowed = false;
	bool brake_allowed = false;
};