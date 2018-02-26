#pragma once

#include "texture.h"
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\mat4x4.hpp>

struct HPIndicator {
	Texture texture;
	GLuint vbo = 0;
	GLuint vao = 0;
	float vertexdata[20] = { -0.947f,  -0.842f, 0.0f,  0.0f, 0.0f,
							 -0.653f,  -0.842f, 0.0f, 0.5f, 0.0f,
							 -0.947f,  -0.758f, 0.0f,  0.0f, 1.0f,
							 -0.653f,  -0.758f, 0.0f, 0.5f, 1.0f };

	HPIndicator();
	~HPIndicator();
	void Bind();
};

