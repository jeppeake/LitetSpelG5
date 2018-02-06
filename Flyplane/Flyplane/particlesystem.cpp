#include "particlesystem.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>

ParticleSystem::ParticleSystem(unsigned particles, float life, float spread, glm::vec4 col, glm::vec4 vel)
{
	numParticles = particles;
	particleLife = life;
	particleSpread = spread;
	particleColor = col;
	program.create("partVert.glsl", "partFrag.glsl");
	{
		std::ifstream file("partCompute.glsl");
		std::string c = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		const char *code = c.c_str();
		shaderID = glCreateShaderProgramv(GL_COMPUTE_SHADER, 1, &code);
		if (!shaderID)
		{
			std::cerr << "Failed to load computeshader \n";
		}
	}
	glGenBuffers(1, &gPos);
	glGenBuffers(1, &gVel);
	glGenBuffers(1, &gCol);
	glGenBuffers(1, &gLife);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gPos);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	GLint access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	glm::vec4 *p = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (particles) * sizeof(glm::vec4), access);

	//Buffer the initial positions
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		p[i] = glm::vec4(0.2,1.0,5.0, 1.0);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gVel);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	//Buffer the initial velocities
	glm::vec4 *v = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, particles * sizeof(glm::vec4), access);
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		v[i] = glm::vec4(0.0, 0.0, 0.0, 0.0);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	///////////////////////////////////////////////
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gLife);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	//Buffer the initial lives
	GLfloat *l = (GLfloat*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, particles * sizeof(GLfloat), access);
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		l[i] = life;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gCol);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	//Buffer the initial color
	glm::vec4 *c = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, particles * sizeof(glm::vec4), access);
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		c[i] = col;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gPos);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, gCol);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::update(float dt, glm::vec3 pos, glm::vec3 direction)
{

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, gPos);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, gVel);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, gLife);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, gCol);

	glUseProgram(shaderID);
	glProgramUniform3fv(shaderID, glGetUniformLocation(shaderID, "spawn"), 1, glm::value_ptr(pos));
	glProgramUniform3fv(shaderID, glGetUniformLocation(shaderID, "direction"), 1, glm::value_ptr(direction));
	glProgramUniform1f(shaderID, glGetUniformLocation(shaderID, "particleLife"), particleLife);
	glProgramUniform1f(shaderID, glGetUniformLocation(shaderID, "dt"), dt);

	glDispatchCompute((numParticles / 128) + 1, 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);
}

void ParticleSystem::render(glm::mat4 view, glm::mat4 proj)
{
	program.use();
	glBindVertexArray(VAO);
	program.uniform("view", view);
	program.uniform("projection", proj);
	glDrawArrays(GL_POINTS, 0, numParticles);
	glBindVertexArray(0);
}
