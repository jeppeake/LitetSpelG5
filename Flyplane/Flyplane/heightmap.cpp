#include "heightmap.h"
#include "lodepng.h"
#include <GL\glew.h>
#include <iostream>
#include "model.h"
Heightmap::Heightmap(const std::string &file) {
	std::vector<unsigned char> img;
	unsigned error = lodepng::decode(img, width, height, file);
	if (error != 0) {
		std::cout << "[ERROR] Failed to load heightmap." << "\n";
	}

	int count = 0;
	for (int i = 0; i < width*height * 4; i+=4) {
		//std::cout << (unsigned int)img[i] << " " << (unsigned int)img[i+1] << " " << (unsigned int)img[i+2] << " " << (unsigned int)img[i + 3] << "\n";
		int sumColors = (255-(unsigned int)img[i]) + (255-(unsigned int)img[i + 1]) + (255-(unsigned int)img[i + 2]) + (255-(unsigned int)img[i + 3]);
		//std::cout << "Color intesnity:" << sumColors << "\n";
		Vertex vertex;
		float x = (count % width) * spread;
		float y = sumColors * hScale;
		float z = (count / width) * spread;
		vertex.position = glm::vec3(x, y, z);
		vertex.tex_coords = glm::vec2(0.0, 0.0);
		vertex.normal = glm::vec3(0.0, 1.0, 0.0);
		vertices.push_back(vertex);
		/*vertices.push_back(sumColors * hScale);
		vertices.push_back((count / width) * spread);
		vertices.push_back()*/
		std::cout << "x= " << (count % width) * spread << " y= " << sumColors/255 * hScale << " z=" << (count / width) * spread << "\n";
		count++;
	}
	
	for (int i = 0; i < width*height-width; i+=1) {
		indices.push_back(i);
		indices.push_back(i + width);
		std::cout << i << " " << i + width << " ";
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
}

void Heightmap::unbind() {
	glBindVertexArray(0);
}