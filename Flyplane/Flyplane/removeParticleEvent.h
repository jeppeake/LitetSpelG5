#pragma once
#include "addParticleEvent.h"
#include <entityx/entityx.h>
struct RemoveParticleEvent {
	PARTICLE_TYPES type;
	entityx::ComponentHandle<ParticleComponent> component;
	RemoveParticleEvent(PARTICLE_TYPES type, entityx::ComponentHandle<ParticleComponent> component) : type(type), component(component) {}
};