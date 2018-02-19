#pragma once
#include "particlecomponent.h"
enum PARTICLE_TYPES
{
	TRAIL,
	EXPLOSION,
	NUM_TYPES
};
struct AddParticleEvent {
	PARTICLE_TYPES type;
	entityx::ComponentHandle<ParticleComponent> component;
	float effectLength = 0;
	AddParticleEvent(PARTICLE_TYPES type, entityx::ComponentHandle<ParticleComponent> component, float effectLength = 0) : type(type), component(component), effectLength(effectLength) {}
};