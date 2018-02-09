#pragma once

#include "texture.h"
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\mat4x4.hpp>

class CrossHair {
private:
	Texture texture;
	GLuint vbo = 0;
	GLuint vao = 0;
	glm::mat4 modleMatrix = glm::translate(glm::vec3(0, -0.08, 0)) *  glm::rotate(3.14f / 4, glm::vec3(0, 0, 1));

	float vertexdata[20] = { -0.02f, -0.02f, -0.9f, 0.f, 0.f,
							  0.02f, -0.02f, -0.9f, 1.f, 0.f,
							 -0.02f,  0.02f, -0.9f, 0.f, 1.f,
							  0.02f,  0.02f, -0.9f, 1.f, 1.f };
public:
	CrossHair();
	~CrossHair();
	void Bind();
	glm::mat4& getMatrix();
	
};
