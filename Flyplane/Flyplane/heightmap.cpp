#include "heightmap.h"
#include "lodepng.h"
#include <GL\glew.h>
#include <glm\gtc\constants.hpp>
#include <iostream>
#include "model.h"
#include "timer.h"

int index(int x, int y, int width) {
	return x + y * width;
}

Heightmap::Heightmap() {}

Heightmap::Heightmap(const std::string &file, const std::string &texFile) {
	loadMap(file, texFile); 
}

void Heightmap::loadMap(const std::string &file, const std::string &texFile) {
	tex.loadTexture(texFile);


	std::vector<unsigned char> img;
	unsigned error = lodepng::decode(img, width, height, file);
	if (error != 0) {
		std::cout << "[ERROR] Failed to load heightmap." << "\n";
	}


	hScale = 2000.f;
	spread = 40.f;


	std::vector<unsigned char> heights;

	for (int ix = 0; ix < width; ix++) {
		for (int iy = 0; iy < height; iy++) {
			int i = ix * 4 + iy * 4 * width;
			float sumColors = 2.f*img[i]/255.f - 1.f;
			
			heights.push_back(img[i]);

			float x = ix * spread;
			float y = sumColors * hScale;
			float z = iy * spread;
			vertices.emplace_back(x, y, z);

			/*
			int house = img[i + 1];
			if (house > 0) {
				std::cout << "Adding house\n";
				houses.emplace_back(glm::vec3(x, y, z), house);
			}
			*/
		}
	}


	numPatchVerts = 63;
	scale = glm::vec3(70, 70, 70);

	std::vector<glm::vec2> uvs;
	for (int y = 0; y < numPatchVerts; y++) {
		for (int x = 0; x < numPatchVerts; x++) {
			uvs.emplace_back(float(x) / (numPatchVerts-1), float(y) / (numPatchVerts - 1));

			if (x < numPatchVerts - 1 && y < numPatchVerts - 1) {
				if (x % 2 == y % 2) {
					indices.push_back((x)+(y)* numPatchVerts);
					indices.push_back((x + 1) + (y + 1)* numPatchVerts);
					indices.push_back((x + 1) + (y)* numPatchVerts);

					indices.push_back((x)+(y)* numPatchVerts);
					indices.push_back((x)+(y + 1)* numPatchVerts);
					indices.push_back((x + 1) + (y + 1)* numPatchVerts);
				} else {
					indices.push_back((x)+(y)* numPatchVerts);
					indices.push_back((x)+(y + 1)* numPatchVerts);
					indices.push_back((x + 1) + (y)* numPatchVerts);

					indices.push_back((x + 1) + (y)* numPatchVerts);
					indices.push_back((x)+(y + 1)* numPatchVerts);
					indices.push_back((x + 1) + (y + 1)* numPatchVerts);
				}
			}
		}
	}


	glGenTextures(1, &heightmapTex);
	glBindTexture(GL_TEXTURE_2D, heightmapTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, &heights[0]);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*uvs.size(), &uvs[0], GL_STATIC_DRAW);

	auto stride = sizeof(glm::vec2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);

	glBindVertexArray(0);
}

void Heightmap::bind() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glActiveTexture(GL_TEXTURE0);
	tex.bind(0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, heightmapTex);
}

void Heightmap::unbind() {
	glBindVertexArray(0);
}

double Heightmap::heightAt(glm::vec3 _pos) {

	auto temp = _pos.x;
	_pos.x = _pos.z;
	_pos.z = temp;

	double height = 0;
	_pos = _pos - this->pos;
	int x = (int)(_pos.x / spread);
	int z = (int)(_pos.z / spread);

	if (x < 0 || x >= width-1 || z < 0 || z >= this->height-1)
		return height;

	glm::vec3 v1 = vertices[index(x, z, width)];
	glm::vec3 v2 = vertices[index(x+1, z, width)];
	glm::vec3 v3 = vertices[index(x, z+1, width)];
	glm::vec3 v4 = vertices[index(x+1, z+1, width)];

	float sqX = (_pos.x / spread) - x;
	float sqZ = (_pos.z / spread) - z;


	if ((sqX + sqZ) < 1)
	{
		height = v1.y;
		height += (v2.y - v1.y) * sqX;
		height += (v3.y - v1.y) * sqZ;
	}
	else
	{
		height = v4.y;
		height += (v2.y - v4.y) * (1.0f - sqZ);
		height += (v3.y - v4.y) * (1.0f - sqX);
	}

	return height;
}



std::vector<Patch> Heightmap::buildPatches(glm::vec3 _pos) {

	std::vector<Patch> result;

	glm::vec2 pos(_pos.x/scale.x, _pos.z/scale.z);

	glm::vec2 offset(0);

	float patchSize = width / 2.f;

	recursiveBuildPatches(result, pos, patchSize, offset, 0);

	return result;
}


void Heightmap::recursiveBuildPatches(std::vector<Patch>& patches, glm::vec2 pos, float patchSize, glm::vec2 offset, int level) {

	int maxLevels = 5;

	for (int i = 0; i < 4; i++) {
		int x = i % 2;
		int z = i / 2;

		glm::vec2 center;
		center.x = offset.x + patchSize * x + patchSize * 0.5f;
		center.y = offset.y + patchSize * z + patchSize * 0.5f;

		float dist = length(pos - center);

		bool is_close = dist < glm::root_two<float>() * patchSize*2.f;

		glm::vec2 new_offset = offset;
		new_offset.x += x * patchSize;
		new_offset.y += z * patchSize;

		if (!is_close) {
			// add patch to list
			patches.emplace_back(patchSize, new_offset);
		} else {
			if (level < maxLevels) {
				// should divide
				recursiveBuildPatches(patches, pos, patchSize*0.5f, new_offset, level + 1);
			} else {
				patches.emplace_back(patchSize, new_offset);
			}	
		}
	}
}