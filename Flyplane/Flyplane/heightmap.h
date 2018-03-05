#pragma once
#include <vector>
#include "model.h"
#include "texture.h"
#include "camera.h"
#include "shader.h"
#include <entityx/entityx.h>
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
	Texture materialMap;
	Texture textures[3];

	unsigned int width, height;
	int numPatchVerts = 7;
	int maxLevels = 3;
	
	float waterHeight = 0;

	GLuint heightmapTex = 0;

	GLuint ebos[9];
	GLuint vbo = 0;
	GLuint vao = 0;

	void recursiveBuildPatches(std::vector<Patch>& patches, float patchSize, glm::vec2 offset, int level, glm::dvec3 normals[4], glm::dvec3 orig);
	void recursiveBuildPatchesOrtho(std::vector<Patch>& patches, float patchSize, glm::vec2 offset, int level, BoundingBox& frustum, glm::dvec3 orig);
	float heightOfWater(glm::vec3 pos);
	void createIndices(int x, int y, int i);

	glm::vec3 normalAt(glm::vec3 pos);
	bool isWater(glm::vec3 pos);
public:
	std::vector<glm::vec3> vertices;

	std::vector<GLuint> indices[9];
	glm::vec3 pos;
	glm::vec3 scale;

	

	Heightmap();
	Heightmap(const std::string &maptxt);
	void loadMap(const std::string &maptxt);
	void loadStructures();
	void buildStructures(entityx::EntityManager &mgr);
	void bind(ShaderProgram& program);
	void unbind();
	double heightAt(glm::vec3 pos);
	bool isOutside(glm::vec3 pos);
	void bindIndices(int i);
	void unbindIndices();
	
	inline glm::vec2 getSize() {
		return glm::vec2(width, height);
	}

	glm::vec3 generateHousePos();

	glm::vec2 getMinMaxHeights();

	std::vector<Patch> buildPatchesOrtho(glm::mat4 viewProj, Camera c);
	std::vector<Patch> buildPatches(Camera camera);

	std::vector<House> getHouses() {
		return houses;
	}
};

int chooseIndices(int x, int y, bool divideLeft, bool divideTop, bool divideRight, bool divideBottom);
