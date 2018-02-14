#pragma once

#include "computeShader.h"
#include "particles.h"

struct ParticleComponent {
	ParticleComponent() {}
	Particles* system;
};