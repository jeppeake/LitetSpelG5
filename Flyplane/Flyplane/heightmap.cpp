#include "heightmap.h"
#include "lodepng.h"
#include <GL\glew.h>
#include <iostream>
#include "model.h"

int index(int x, int y, int width) {
	return x + y * width;
}

Heightmap::Heightmap(const std::string &file) {
	loadMap(file);
}

Heightmap::Heightmap(const std::string &file, const std::string &texFile) {
	loadMap(file); //load heightmap
	//load texture
	tex = Texture();
	tex.loadTexture(texFile);
}

void Heightmap::loadMap(const std::string &file) {
	std::vector<unsigned char> img;
	unsigned error = lodepng::decode(img, width, height, file);
	if (error != 0) {
		std::cout << "[ERROR] Failed to load heightmap." << "\n";
	}

	for (int ix = 0; ix < width; ix++) {
		for (int iy = 0; iy < height; iy++) {
			int i = ix * 4 + iy * 4 * width;
			int sumColors =  (unsigned int)img[i] + (unsigned int)img[i + 1] +  (unsigned int)img[i + 2] + (unsigned int)img[i + 3];
			//std::cout << "Color intesnity:" << sumColors << "\n";
			Vertex vertex;
			float x = ix * spread;
			float y = sumColors * hScale;
			float z = iy * spread;
			vertex.position = glm::vec3(x, y, z);
			vertex.tex_coords = glm::vec2((x / spread), (z / spread));
			vertex.normal = glm::vec3(0.0, 1.0, 0.0);
			vertices.push_back(vertex);
		}
	}

	for (int x = 0; x < width-1; x++) {
		for (int y = 0; y < height-1; y++) {
			indices.push_back(index(x, y, width));
			indices.push_back(index(x + 1, y, width));
			indices.push_back(index(x+1, y+1, width));

			indices.push_back(index(x, y, width));
			indices.push_back(index(x + 1, y + 1, width));
			indices.push_back(index(x, y + 1, width));
			
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);


	unsigned int stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(2 * sizeof(glm::vec3)));


	glBindVertexArray(0);
}

void Heightmap::bind() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	tex.bind(0);
}

void Heightmap::unbind() {
	glBindVertexArray(0);
}