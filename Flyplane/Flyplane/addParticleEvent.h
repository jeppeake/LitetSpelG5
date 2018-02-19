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
	AddParticleEvent(PARTICLE_TYPES type, entityx::ComponentHandle<ParticleComponent> component) : type(type), component(component) {}
};