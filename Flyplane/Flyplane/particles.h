#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <entityx\entityx.h>
#include "shader.h"
#include "computeShader.h"
#include "addParticleEvent.h"
#include "timer.h"
#include "texture.h"
#include "assetloader.h"
class Particles
{
private:
	float effectTime;
	unsigned numParticles;
	GLuint gPos, gColor, gLife, gVel, VAO;
	ComputeShader compute;
	Texture *text = nullptr;
	float size = 0.1;
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
	void setSize(float size)
	{
		this->size = size;
	}
	//Kommer kanske sen...
	void setTexture(const std::string &name)
	{
		/*
		text = AssetLoader::getLoader().getTexture(name);
		if (!text)
		{
			AssetLoader::getLoader().loadTexture("assets/Textures/" + name + ".png", name);
			text = AssetLoader::getLoader().getTexture(name);
			if (!text)
			{
				std::cout << "Failed to load texture for particles.. \n";
				text = nullptr;
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, text->getID());
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
		}
		*/
	}
	void update();
	void render();
	bool isAlive();
	// used for pool in particlesystem.h
	bool used = false;
	ShaderProgram* program;
	PARTICLE_TYPES type;
};