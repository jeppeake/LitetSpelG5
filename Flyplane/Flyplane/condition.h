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

	virtual bool test(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) = 0; //add player, friendlies, (hostile) missiles and aircraft orientation to function call

};