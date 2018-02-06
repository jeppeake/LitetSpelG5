#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.h"
const unsigned maxParticles = 10000;
class ParticleSystem
{
private:
	unsigned numParticles;
	float particleLife, particleSpread;
	glm::vec4 particleColor;
	GLuint gPos, gVel, gLife, gCol, VAO;
	glm::vec4 color;
	GLuint shaderID;
	ShaderProgram program;
public:
	ParticleSystem(unsigned particles, float life, float spread, glm::vec4 col, glm::vec4 vel);
	void update(float dt, glm::vec3 pos, glm::vec3 direction);
	void render(glm::mat4 view, glm::mat4 proj);
};