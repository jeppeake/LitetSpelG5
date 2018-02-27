#include <random>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include "particles.h"
#include "renderer.h"

Particles::Particles(unsigned particles, float _effectTime)
{
	this->effectTime = _effectTime;
	this->t.restart();
	srand(time(NULL));
	numParticles = particles;
	glGenBuffers(1, &gPos);
	glGenBuffers(1, &gColor);
	glGenBuffers(1, &gLife);
	glGenBuffers(1, &gVel);
	glGenVertexArrays(1, &VAO);

	std::cout << "[DEBUG] Creating Particles, count: " << numParticles << ", id's: " << gPos << ", " << gColor << ", " << gLife << ", " << gVel << "\n";

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
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glm::vec4 *v = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (numParticles) * sizeof(glm::vec4), access);


	//Buffer the initial velocities
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		v[i] = glm::vec4();
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	///////////////////////////////////////////////



	///////////////////////////////////////////
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gLife);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	//Buffer the initial lives
	GLfloat *l = (GLfloat*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numParticles * sizeof(GLfloat), access);

	for (unsigned i = 0; i < this->numParticles; i++)
	{
		l[i] = 0;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gColor);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	//Buffer the initial color
	glm::vec4 *c = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numParticles * sizeof(glm::vec4), access);
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		c[i] = glm::vec4();
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

Particles::~Particles() {
	glDeleteBuffers(1, &gPos);
	glDeleteBuffers(1, &gColor);
	glDeleteBuffers(1, &gLife);
	glDeleteBuffers(1, &gVel);
	glDeleteVertexArrays(1, &VAO);
}

void Particles::update()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, gPos);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, gVel);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, gLife);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, gColor);

	//Dispatch
	glDispatchCompute((numParticles / 128) + 1, 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);
}

void Particles::render(ShaderProgram& program)
{
	auto camera = Renderer::getRenderer().getCamera();
	auto transform = camera.getTransform();
	program.use();
	glBindVertexArray(VAO);
	/*
	if (text)
	{
		glActiveTexture(GL_TEXTURE0);
		text->bind(GL_TEXTURE0);
	}
	*/
	program.uniform("view", camera.getViewMatrix());
	program.uniform("projection", camera.getProjMatrix());
	program.uniform("cPos", transform.pos);
	program.uniform("cUp", transform.orientation * glm::vec3(0.0, 1.0, 0.0));
	program.uniform("particleSize", size);
	glDrawArrays(GL_POINTS, 0, numParticles);
	glBindVertexArray(0);
}

bool Particles::isAlive()
{
	if (!effectTime)
		return true;
	double elapsed = t.elapsed();
	if (elapsed >= effectTime)
	{
		t.restart();
		return false;
	}
	else
	{
		return true;
	}
}
