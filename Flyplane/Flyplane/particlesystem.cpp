#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include "particlesystem.h"
#include "renderer.h"
ParticleSystem::ParticleSystem(unsigned particles, float life, float size, glm::vec3 color)
{
	srand(time(NULL));
	numParticles = particles;
	this->life = life;
	this->size = size;
	glGenBuffers(1, &gPos);
	glGenBuffers(1, &gColor);
	glGenBuffers(1, &gLife);
	glGenBuffers(1, &gVel);
	glGenVertexArrays(1, &VAO);
	{
		std::ifstream file("partCompute.glsl");
		std::string c = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		const char *code = c.c_str();
		cs = glCreateShaderProgramv(GL_COMPUTE_SHADER, 1, &code);
	}
	program.create("partVert.glsl", "partGeom.glsl", "partFrag.glsl");
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gPos);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	GLint access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	glm::vec4 *p = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (numParticles) * sizeof(glm::vec4), access);

	//Buffer the initial positions
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		p[i] = glm::vec4(0.0,0.0,0.0, 1.0);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gVel);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	//Buffer the initial lives
	GLfloat *v = (GLfloat*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numParticles * sizeof(GLfloat), access);
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		v[i] = rand() % 15 + 1;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	///////////////////////////////////////////////
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gLife);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	//Buffer the initial lives
	GLfloat *l = (GLfloat*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numParticles * sizeof(GLfloat), access);
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		l[i] = rand() % (int)this->life + 1;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gColor);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	//Buffer the initial color
	glm::vec4 *c = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numParticles * sizeof(glm::vec4), access);
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		c[i] = glm::vec4(color, 1.0);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	//set up the VAO for the particles.
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gPos);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, gColor);
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
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, gColor);

	//Set the compute-shader
	glUseProgram(cs);
	//Set the uniforms
	glProgramUniform3fv(cs, glGetUniformLocation(cs, "spawn"), 1, glm::value_ptr(pos));
	glProgramUniform3fv(cs, glGetUniformLocation(cs, "direction"), 1, glm::value_ptr(direction));
	glProgramUniform1fv(cs, glGetUniformLocation(cs, "life"), 1, &life);
	glProgramUniform1fv(cs, glGetUniformLocation(cs, "dt"), 1, &dt);
	//Dispatch
	glDispatchCompute((numParticles / 128) + 1, 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);
}

void ParticleSystem::render()
{
	auto camera = Renderer::getRenderer().getCamera();
	auto transform = camera.getTransform();
	program.use();
	glBindVertexArray(VAO);
	program.uniform("view", camera.getViewMatrix());
	program.uniform("projection", camera.getProjMatrix());
	program.uniform("cPos", transform.pos);
	program.uniform("cUp", glm::toMat3(transform.orientation) * glm::vec3(0.0, 1.0, 0.0));
	program.uniform("particleSize", size);
	glDrawArrays(GL_POINTS, 0, numParticles);
	glBindVertexArray(0);
}
