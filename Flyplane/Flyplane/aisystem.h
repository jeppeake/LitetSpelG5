#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "aicomponent.h"
#include "transform.h"
#include "flightcomponent.h"
#include "playercomponent.h"
#include "terraincomponent.h"
#include "behaviour.h"
#include "equipment.h"
#include <iostream>

using namespace entityx;

struct AISystem : public entityx::System<AISystem> {

	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override;
};