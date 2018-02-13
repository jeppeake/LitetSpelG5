#include "heightmap.h"
#include "lodepng.h"
#include <GL\glew.h>
#include <glm\gtc\constants.hpp>
#include <glm\gtx\intersect.hpp>
#include <iostream>
#include "model.h"
#include "timer.h"
#include "camera.h"
#include <fstream>
#include "shader.h"
#include <stdlib.h>

int index(int x, int y, int width) {
	return x + y * width;
}

glm::vec3 normal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	auto v1 = p1 - p2;
	auto v2 = p3 - p2;
	return normalize(cross(v1, v2));
}

glm::vec3 normal(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3) {
	return normal(glm::vec3(p1), glm::vec3(p2), glm::vec3(p3));
}


Heightmap::Heightmap() {}

Heightmap::Heightmap(const std::string &maptxt) {
	loadMap(maptxt);
}

void Heightmap::loadMap(const std::string &maptxt) {
	std::ifstream f(maptxt);

	std::string heightmap;
	std::string materialmap;
	std::string mat1; 
	std::string mat2;
	std::string mat3;

	std::getline(f, heightmap);
	std::getline(f, materialmap);
	std::getline(f, mat1);
	std::getline(f, mat2);
	std::getline(f, mat3);

	textures[0].loadTexture(mat1);
	textures[1].loadTexture(mat2);
	textures[2].loadTexture(mat3);
	materialMap.loadTexture(materialmap);

	std::vector<unsigned char> img;
	unsigned error = lodepng::decode(img, width, height, heightmap, LCT_RGBA, 16U);
	if (error != 0) {
		std::cout << "[ERROR] Failed to load heightmap '" << heightmap << "': " << lodepng_error_text(error) << "\n";
		system("pause");
		std::exit(EXIT_FAILURE);
	}

	std::vector<uint16_t> heights;

	for (int iy = 0; iy < height; iy++) {
		for (int ix = 0; ix < width; ix++) {
			int i = index(ix * 8, iy, 8 * width);
			
			uint16_t a = img[i];
			uint16_t b = img[i+1];
			uint16_t red = (a << 8) | b;
			double sample = red;

			//(img[i] / 255.0)*double(std::numeric_limits<unsigned int>::max());
			
			heights.push_back(sample);

			float x = ix;
			float y = sample*255.0/double(std::numeric_limits<uint16_t>::max());
			float z = iy;
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
	scale = 4.f*glm::vec3(3, 5, 3);

	std::vector<glm::vec2> uvs;
	for (int y = 0; y < numPatchVerts; y++) {
		for (int x = 0; x < numPatchVerts; x++) {
			uvs.emplace_back(float(x) / (numPatchVerts-1), float(y) / (numPatchVerts - 1));

			if (x < numPatchVerts - 1 && y < numPatchVerts - 1) {
				for (int i = 0; i < 9; i++) {
					createIndices(x, y, i);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, &heights[0]);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(9, ebos);
	for (int i = 0; i < 9; i++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[i].size() * sizeof(GLuint), &indices[i][0], GL_STATIC_DRAW);
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*uvs.size(), &uvs[0], GL_STATIC_DRAW);

	auto stride = sizeof(glm::vec2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);

	glBindVertexArray(0);
}

void Heightmap::bind(ShaderProgram& shader) {
	shader.uniform("scale", scale);
	shader.uniform("heightmapSize", getSize());
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	shader.uniform("materialmap", 0);
	materialMap.bind(0);
	for (size_t i = 0; i < 3; i++) {
		int slot = i + 3;
		shader.uniform("material" + std::to_string(i+1), slot);
		textures[i].bind(slot);
	}

	shader.uniform("heightmap", 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, heightmapTex);
}

void Heightmap::unbind() {
	glBindVertexArray(0);
}

double Heightmap::heightAt(glm::vec3 _pos) {
	double height = 0;
	int x = (int)(_pos.x / scale.x);
	int z = (int)(_pos.z / scale.z);

	if (x < 0 || x >= width-1 || z < 0 || z >= this->height-1)
		return height;

	glm::vec3 v1 = vertices[index(x, z, width)];
	glm::vec3 v2 = vertices[index(x+1, z, width)];
	glm::vec3 v3 = vertices[index(x, z+1, width)];
	glm::vec3 v4 = vertices[index(x+1, z+1, width)];

	float sqX = (_pos.x / scale.x) - x;
	float sqZ = (_pos.z / scale.z) - z;


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

	return scale.y * height;
}

void Heightmap::bindIndices(int i) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[i]);
}

void Heightmap::unbindIndices() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



std::vector<Patch> Heightmap::buildPatches(Camera camera) {
	std::vector<Patch> result;

	auto inverse = camera.getInverse();
	
	glm::dvec3 origin = camera.getTransform().pos;

	glm::dvec3 farPlane[4];
	glm::dmat4 farPlane4;
	farPlane4[0] = glm::dvec4(-1, -1, 1, 1);
	farPlane4[1] = glm::dvec4(1, -1, 1, 1);
	farPlane4[2] = glm::dvec4(-1, 1, 1, 1);
	farPlane4[3] = glm::dvec4(1, 1, 1, 1);
	for (int i = 0; i < 4; i++) {
		farPlane4[i] = inverse * farPlane4[i];
		farPlane4[i] /= farPlane4[i].w;

		farPlane[i] = origin + 10'000'000.0 * normalize(glm::dvec3(farPlane4[i]) - origin);
	}

	glm::vec2 offset(-(width / 2.f));
	float patchSize = width; // / 2.f;
	recursiveBuildPatches(result, patchSize, offset, 0, farPlane, origin);

	return result;
}


void Heightmap::recursiveBuildPatches(std::vector<Patch>& patches, float patchSize, glm::vec2 offset, int level, glm::dvec3 farPlane[4], glm::dvec3 orig) {

	int maxLevels = 8;


	glm::vec2 pos(orig.x/scale.x, orig.z/scale.z);

	// better read as "left divides" etc
	bool divideLeft = false;
	bool divideRight = false;
	bool divideTop = false;
	bool divideBottom = false;


	float neighbourSize = patchSize * 2.f;
	float len = glm::root_two<float>() * neighbourSize*2.f;
	float dist;
	glm::vec2 center;

	auto checkDivide = [&](glm::vec2 d) {
		glm::vec2 off = offset;
		off += neighbourSize * d;
		center = off + neighbourSize * glm::vec2(0.5f);
		dist = length(pos - center);
		return dist < len;
	};

	divideLeft = checkDivide(glm::vec2(-1, 0));
	divideRight = checkDivide(glm::vec2(1, 0));
	divideTop = checkDivide(glm::vec2(0, -1));
	divideBottom = checkDivide(glm::vec2(0, 1));

	for (int i = 0; i < 4; i++) {
		int x = i % 2;
		int y = i / 2;


		glm::vec2 center = offset + patchSize * glm::vec2(x + 0.5f, y + 0.5f);

		float dist = length(pos - center);
		float len = glm::root_two<float>() * patchSize*2.f;

		bool is_close = dist < len;

		glm::vec2 new_offset = offset;
		new_offset.x += x * patchSize;
		new_offset.y += y * patchSize;

		int indices = chooseIndices(x, y, divideLeft, divideTop, divideRight, divideBottom);

		if (level < maxLevels && is_close) {
			// should divide
			recursiveBuildPatches(patches, patchSize*0.5f, new_offset, level + 1, farPlane, orig);
		} else {

			bool intersection = false;

			
			double leftMax = -std::numeric_limits<double>::max();
			double topMax = -std::numeric_limits<double>::max();
			double rightMax = -std::numeric_limits<double>::max();
			double bottomMax = -std::numeric_limits<double>::max();

			double leftMin = std::numeric_limits<double>::max();
			double topMin = std::numeric_limits<double>::max();
			double rightMin = std::numeric_limits<double>::max();
			double bottomMin = std::numeric_limits<double>::max();
			// check all corners of the patch
			for (int iy = -1; iy <= 1; iy += 2) {
				for (int ix = -1; ix <= 1; ix += 2) {
					glm::dvec3 pos;

					glm::dvec3 center3 = glm::dvec3(scale) * glm::dvec3(center.x + ix* double(patchSize)*0.5, 0, center.y + iy * double(patchSize)*0.5);

					//left
					if (glm::intersectLineTriangle(center3, glm::dvec3(0, 1, 0), orig, farPlane[1], farPlane[3], pos)) {
						intersection = true;
						leftMax = glm::max(pos.x, leftMax);
						leftMin = glm::min(pos.x, leftMin);
					}
					
					
					//top
					if (glm::intersectLineTriangle(center3, glm::dvec3(0, 1, 0), orig, farPlane[3], farPlane[2], pos)) {
						intersection = true;
						topMax = glm::max(pos.x, topMax);
						topMin = glm::min(pos.x, topMin);
					}

					//right
					if (glm::intersectLineTriangle(center3, glm::dvec3(0, 1, 0), orig, farPlane[2], farPlane[0], pos)) {
						intersection = true;
						rightMax = glm::max(pos.x, rightMax);
						rightMin = glm::min(pos.x, rightMin);
					}

					//bottom
					if (glm::intersectLineTriangle(center3, glm::dvec3(0, 1, 0), orig, farPlane[0], farPlane[1], pos)) {
						intersection = true;
						bottomMax = glm::max(pos.x, bottomMax);
						bottomMin = glm::min(pos.x, bottomMin);
					}
				}
			}

			bool cornerAbove = leftMax > 0 || topMax > 0 || rightMax > 0 || bottomMax > 0;

			double maxHeight = 255.0*double(scale.y);
			bool cornerBelow = leftMin < maxHeight || topMin < maxHeight || rightMin < maxHeight || bottomMin < maxHeight;


			if (intersection && cornerAbove && cornerBelow)
				patches.emplace_back(patchSize, new_offset, indices);
		}
	}
}

int chooseIndices(int x, int y, bool divideLeft, bool divideTop, bool divideRight, bool divideBottom) {
	int indices = 0;
	if (x == 0 && y == 0) {
		if (!divideLeft && !divideTop) {
			indices = 6;
		} else if (!divideLeft) {
			indices = 2;
		} else if (!divideTop) {
			indices = 3;
		}
	} else if (x == 1 && y == 0) {
		if (!divideTop && !divideRight) {
			indices = 7;
		} else if (!divideTop) {
			indices = 3;
		} else if (!divideRight) {
			indices = 4;
		}
	} else if (x == 0 && y == 1) {
		if (!divideLeft && !divideBottom) {
			indices = 5;
		} else if (!divideLeft) {
			indices = 2;
		} else if (!divideBottom) {
			indices = 1;
		}
	} else {
		if (!divideRight && !divideBottom) {
			indices = 8;
		} else if (!divideRight) {
			indices = 4;
		} else if (!divideBottom) {
			indices = 1;
		}
	}
	return indices;
}

// pls
void Heightmap::createIndices(int x, int y, int i) {
	if (x % 2 == y % 2) {
		//1
		// i == 3, 4, 6, 7, 8
		if (y == 0 && (i == 3 || i == 6 || i == 7)) {
			// up
			indices[i].push_back(index(x, y, numPatchVerts));
			indices[i].push_back(index(x + 1, y + 1, numPatchVerts));
			indices[i].push_back(index(x + 2, y, numPatchVerts));
		} else if (x == numPatchVerts - 2 && (i == 4 || i == 7 || i == 8)) {
			// right
			indices[i].push_back(index(x, y, numPatchVerts));
			indices[i].push_back(index(x + 1, y + 1, numPatchVerts));
			indices[i].push_back(index(x + 1, y - 1, numPatchVerts));
		} else {
			indices[i].push_back(index(x, y, numPatchVerts));
			indices[i].push_back(index(x + 1, y + 1, numPatchVerts));
			indices[i].push_back(index(x + 1, y, numPatchVerts));
		}

		//2
		// left == 2, 5, 6
		// down == 1, 5, 8
		if (x == 0 && (i == 2 || i == 5 || i == 6)) {
			// left
			indices[i].push_back(index(x, y, numPatchVerts));
			indices[i].push_back(index(x, y + 2, numPatchVerts));
			indices[i].push_back(index(x + 1, y + 1, numPatchVerts));
		} else if (y == numPatchVerts - 2 && (i == 1 || i == 5 || i == 8)) {
			// down
			// skip
		} else {
			indices[i].push_back(index(x, y, numPatchVerts));
			indices[i].push_back(index(x, y + 1, numPatchVerts));
			indices[i].push_back(index(x + 1, y + 1, numPatchVerts));
		}

	} else {
		//3
		// left: 2, 5, 6
		// up:   3, 6, 7
		if (x == 0 && (i == 2 || i == 5 || i == 6)) {
			// left
			// skip
		} else if (y == 0 && (i == 3 || i == 6 || i == 7)) {
			// up
			// skip
		} else {
			indices[i].push_back(index(x, y, numPatchVerts));
			indices[i].push_back(index(x, y + 1, numPatchVerts));
			indices[i].push_back(index(x + 1, y, numPatchVerts));
		}

		//4
		// i == 1, 4, 5, 7, 8
		if (y == numPatchVerts - 2 && (i == 1 || i == 5 || i == 8)) {
			// down
			indices[i].push_back(index(x + 1, y, numPatchVerts));
			indices[i].push_back(index(x, y + 1, numPatchVerts));
			indices[i].push_back(index(x + 2, y + 1, numPatchVerts));
		} else if (x == numPatchVerts - 2 && (i == 4 || i == 7 || i == 8)) {
			// right
			// skip
		} else {
			indices[i].push_back(index(x + 1, y, numPatchVerts));
			indices[i].push_back(index(x, y + 1, numPatchVerts));
			indices[i].push_back(index(x + 1, y + 1, numPatchVerts));
		}
	}
}
