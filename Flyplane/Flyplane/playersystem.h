#pragma once


#include <entityx\entityx.h>
#include <entityx\Entity.h>

#include "renderer.h"
#include "modelcomponent.h"
#include "transform.h"

#include "physics.h"
#include "playercomponent.h"
#include "flightcomponent.h"

#include "input.h "
#include "particles.h"

#include "SAIB.h"
using namespace entityx;

struct PlayerSystem : public System<PlayerSystem> {
	sf::Sound warningSound;
	Particles *S;
	PlayerSystem();
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

};