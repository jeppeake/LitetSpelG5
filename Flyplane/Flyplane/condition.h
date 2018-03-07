#pragma once

#include "transform.h"
#include "flightcomponent.h"
#include "terraincomponent.h"
#include "SAIB.h"
#include <entityx\entityx.h>
#include <entityx\Entity.h>


using namespace entityx;
class Condition {
public:
	Condition() {
	}

	virtual bool test(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest) = 0;

};