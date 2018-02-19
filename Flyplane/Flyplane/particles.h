#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <entityx\entityx.h>
#include "shader.h"
#include "computeShader.h"
#include "addParticleEvent.h"
#include "timer.h"
class Particles
{
private:
	float effectTime;
	unsigned numParticles;
	GLuint gPos, gColor, gLife, gVel, VAO;
	ComputeShader compute;
public:
	Timer t;
	Particles(unsigned particles, float _effectTime = 0);
	void setComputeShader(ComputeShader &compute)
	{
		this->compute = compute;
	}
	void setTimer(float _effectTime)
	{
		effectTime = _effectTime;
	}
	void update();
	void render();
	bool isAlive();
	// used for pool in particlesystem.h
	bool used = false;
	ShaderProgram* program;
	PARTICLE_TYPES type;
};