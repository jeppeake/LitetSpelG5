#pragma once
#include "condition.h"
class Always_True : public Condition {
public:
	Always_True() : Condition() {

	}
	bool test(ComponentHandle<Transform> p_transform, ComponentHandle<FlightComponent> p_flight, ComponentHandle<Transform> me_transform, ComponentHandle<FlightComponent> me_flight, bool is_targeted) {
		return true;
	}
};