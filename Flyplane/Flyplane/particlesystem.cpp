#include <iostream>
#include <fstream>
#include "particlesystem.h"
#include "renderer.h"
ParticleSystem::ParticleSystem()
{
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
		if (!cs)
		{
			std::cerr << "Failed to load computeshader \n";
		}
	}
	program.create("partVert.glsl", "partGeom.glsl", "partFrag.glsl");
}

void ParticleSystem::update()
{

}

void ParticleSystem::render()
{
	auto camera = Renderer::getRenderer().getCamera();
	auto transform = camera.getTransform();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, numParticles);
	glBindVertexArray(0);
}
