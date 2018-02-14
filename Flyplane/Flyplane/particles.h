#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.h"
#include "computeShader.h"

class Particles
{
private:
	unsigned numParticles;
	GLuint gPos, gColor, gLife, gVel, VAO;
public:
	Particles(unsigned particles);
	void update(ComputeShader& compute);
	void render();

	// used for pool in particlesystem.h
	bool used = false;
	ShaderProgram* program;
};