#pragma once
#include <vector>
#include "model.h"
#include "texture.h"
#include "camera.h"

struct House {
	House(glm::vec3 pos, unsigned int type) : pos(pos), type(type) {}
	glm::vec3 pos;
	unsigned int type;
};


// Data for drawing a single patch/part of the terrain
struct Patch {
	Patch(float size, glm::vec2 offset, int indices) : size(size), offset(offset), indices(indices) {}
	float size;
	glm::vec2 offset;
	int indices = 0;
};

class Heightmap {
private:
	std::vector<House> houses;

	std::vector<unsigned char> heightmap;
	Texture tex;

	

	unsigned int width, height;
	float hScale = 1000.0f;
	float spread = 20.0f;
	int numPatchVerts;


	GLuint heightmapTex = 0;

	GLuint ebos[9];
	GLuint vbo = 0;
	GLuint vao = 0;


	void recursiveBuildPatches(std::vector<Patch>& patches, glm::vec2 pos, float patchSize, glm::vec2 offset, int level, glm::mat4 viewProj);
	void createIndices(int x, int y, int i);
public:
	std::vector<glm::vec3> vertices;

	std::vector<GLuint> indices[9];
	glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 scale;



	Heightmap();
	Heightmap(const std::string &file, const std::string &texFile);
	void loadMap(const std::string &file, const std::string &texFile);
	void bind();
	void unbind();
	double heightAt(glm::vec3 pos);
	void bindIndices(int i);
	void unbindIndices();
	

	std::vector<Patch> buildPatches(glm::vec3 pos, Camera camera);

	std::vector<House> getHouses() {
		return houses;
	}
};