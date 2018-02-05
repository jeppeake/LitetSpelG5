#pragma once

#include "transform.h"
#include "flightcomponent.h"
#include "SAIB.h"
#include <entityx\entityx.h>
#include <entityx\Entity.h>


using namespace entityx;
class Condition {
public:
	Condition() {
	}

	virtual bool test(ComponentHandle<Transform> p_transform, ComponentHandle<FlightComponent> p_flight, ComponentHandle<Transform> me_transform, ComponentHandle<FlightComponent> me_flight, bool is_targeted) = 0; //add player, friendlies, (hostile) missiles and aircraft orientation to function call

};