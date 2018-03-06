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
	WING_TRAIL,
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

		struct Sparks {
			glm::vec3 pos;
		} sparks;

		struct WingTrail {
			glm::vec3 offset;
			int respawnCounter;
		} wingTrail;
	};
	float distFromCam = 0;
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