#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "transform.h"
#include "physics.h"
#include "input.h"
#include "flightcomponent.h"
#include <iostream>
#include <glm/gtx/projection.hpp>

using namespace entityx;

struct FlightSystem : public entityx::System<FlightSystem> {
	FlightSystem();
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override;
};