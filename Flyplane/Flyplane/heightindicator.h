#pragma once

#include "texture.h"
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\mat4x4.hpp>

struct HeightIndicator {
	Texture texture;
	GLuint vbo = 0;
	GLuint vao = 0;
	float vertexdata[20] = { -0.9f, -0.6f, 0.0f, 0.0f, 0.0f,
							 -0.787f, -0.6f, 0.0f, 1.0f, 0.0f,
							 -0.9f,  0.4f, 0.0f, 0.0f, 3.0f / 23.0f,
							 -0.787f,  0.4f, 0.0f, 1.0f, 3.0f / 23.0f };


	HeightIndicator();
	~HeightIndicator();
	void Bind();
};
