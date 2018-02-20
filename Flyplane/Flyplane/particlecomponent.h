#pragma once
#include <vector>
#include "computeShader.h"
#include <bitset>
class Particles;
struct ParticleComponent {
	ParticleComponent() {}
	std::vector<Particles*> systems;
};