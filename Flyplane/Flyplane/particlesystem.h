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
	GLfloat life, size;
	ShaderProgram program;
public:
	ParticleSystem(unsigned particles, float life, float size, glm::vec3 color, std::string path = "");
	void update(float dt, glm::vec3 pos, glm::vec3 direction);
	void render();
};