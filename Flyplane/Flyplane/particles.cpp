#include <random>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include "particles.h"
#include "renderer.h"

GLint createShader(const char *c)
{
	const GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	if (shader) {
		glShaderSource(shader, 1, &c, NULL);
		glCompileShader(shader);
		const GLuint program = glCreateProgram();
		if (program) {
			GLint compiled = GL_FALSE;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
			if (compiled) {
				glAttachShader(program, shader);
				glLinkProgram(program);
				glDetachShader(program, shader);
			}
			else
			{
				GLint success = 0;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (success == GL_FALSE)
				{
					GLint log_size = 0;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
					std::vector<GLchar> error(log_size);
					glGetShaderInfoLog(shader, log_size, &log_size, &error[0]);
					std::string errorstr{ &error[0] };

					std::cout << "Error in ComputeShader:\n" << errorstr << "\n";

					glDeleteShader(shader);
					system("pause");
					exit(1);
				}
			}
		}
		glDeleteShader(shader);
		return program;
	}
	else {
		return 0;
	}
}
Particles::Particles(unsigned particles)
{
	srand(time(NULL));
	numParticles = particles;
	glGenBuffers(1, &gPos);
	glGenBuffers(1, &gColor);
	glGenBuffers(1, &gLife);
	glGenBuffers(1, &gVel);
	glGenVertexArrays(1, &VAO);

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

	/*
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0, life);
	std::uniform_real_distribution<float> dis2(0.0, 2);
	*/

	//Buffer the initial velocities
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		//v[i] = glm::vec4(dis2(gen), dis2(gen), dis2(gen),1.0);
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
		//l[i] = dis(gen);
		l[i] = 0;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gColor);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	//Buffer the initial color
	glm::vec4 *c = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numParticles * sizeof(glm::vec4), access);
	for (unsigned i = 0; i < this->numParticles; i++)
	{
		//c[i] = glm::vec4(color, 1.0);
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

void Particles::update(ComputeShader& compute)
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

void Particles::render()
{
	float size = 1.f;

	auto camera = Renderer::getRenderer().getCamera();
	auto transform = camera.getTransform();
	program->use();
	glBindVertexArray(VAO);
	program->uniform("view", camera.getViewMatrix());
	program->uniform("projection", camera.getProjMatrix());
	program->uniform("cPos", transform.pos);
	program->uniform("cUp", transform.orientation * glm::vec3(0.0, 1.0, 0.0));
	program->uniform("particleSize", size);
	glDrawArrays(GL_POINTS, 0, numParticles);
	glBindVertexArray(0);
}