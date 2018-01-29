#pragma once
#include <vector>
#include "model.h"
#include "texture.h"
class Heightmap {
private:
	std::vector<unsigned char> heightmap;
	Texture tex;
	unsigned int width, height;
	float hScale = 1.0f;
	float spread = 10.0f;
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
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	double heightAt(glm::vec3 pos);
};