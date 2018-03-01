#pragma once
#include "particlecomponent.h"
enum PARTICLE_TYPES
{
	TRAIL,
	DEAD_TRAIL,
	EXPLOSION,
	SPARKS,
	ENGINE_TRAIL,
	SPEED_PARTICLE,
	NUM_TYPES
};

struct ParticleParameters {
	ParticleParameters() {}
	union {
		struct Explosion {
			float radius;
		} explosion;

		struct EngineTrail {
			glm::vec3 offset;
			float radius;
		} engineTrail;
	};
	float effectLength = 0;
};

struct AddParticleEvent {
	ParticleParameters params;

	PARTICLE_TYPES type;
	entityx::ComponentHandle<ParticleComponent> component;
	AddParticleEvent(PARTICLE_TYPES type, 
	                 entityx::ComponentHandle<ParticleComponent> component, 
	                 ParticleParameters params = {}): type(type), component(component), params(params) {}
};