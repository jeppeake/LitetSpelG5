#pragma once
#include <vector>
#include "model.h"
#include "texture.h"
class Heightmap {
private:
	std::vector<unsigned char> heightmap;
	Texture tex;
	unsigned int width, height;
	float hScale = 0.04f;
	float spread = 0.5f;
	GLuint ebo = 0;
	GLuint vbo = 0;
	GLuint vao = 0;
public:
	Heightmap(const std::string &file);
	Heightmap(const std::string &file, const std::string &texFile);
	void loadMap(const std::string &file);
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	void bind();
	void unbind();
	glm::vec3 pos = glm::vec3(-100.0f, -50.0f, -100.0f);
};