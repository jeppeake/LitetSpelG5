#pragma once
#include <vector>
#include "model.h"
class Heightmap {
private:
	std::vector<unsigned char> heightmap;
	unsigned int width, height;
	float hScale = 0.01f;
	float spread = 3.0f;
	GLuint ebo = 0;
	GLuint vbo = 0;
	GLuint vao = 0;
public:
	Heightmap(const std::string &file);
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	void bind();
	void unbind();
	glm::vec3 pos = glm::vec3(0.0f, -5.0f, 0.0f);
};