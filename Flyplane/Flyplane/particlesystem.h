#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.h"

class ParticleSystem
{
private:
	unsigned numParticles;
	GLuint cs;
	GLuint gPos, gColor, gLife, gVel, VAO;
	ShaderProgram program;
public:
	ParticleSystem();
	void update();
	void render();
};