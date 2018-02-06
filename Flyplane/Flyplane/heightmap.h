#pragma once
#include <vector>
#include "model.h"
#include "texture.h"

struct House {
	House(glm::vec3 pos, unsigned int type) : pos(pos), type(type) {}
	glm::vec3 pos;
	unsigned int type;
};

class Heightmap {
private:
	std::vector<House> houses;

	std::vector<unsigned char> heightmap;
	Texture tex;

	

	unsigned int width, height;
	float hScale = 1000.0f;
	float spread = 20.0f;


	GLuint heightmapTex = 0;

	GLuint ebo = 0;
	GLuint vbo = 0;
	GLuint vao = 0;
public:
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);

	Heightmap();
	Heightmap(const std::string &file, const std::string &texFile);
	void loadMap(const std::string &file, const std::string &texFile);
	void bind();
	void unbind();
	double heightAt(glm::vec3 pos);

	std::vector<House> getHouses() {
		return houses;
	}
};