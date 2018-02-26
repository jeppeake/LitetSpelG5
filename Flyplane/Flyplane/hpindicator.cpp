#include "hpindicator.h"
#include <iostream>

HPIndicator::HPIndicator() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertexdata, GL_STATIC_DRAW);


	unsigned int stride = 5 * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 3));

	glBindVertexArray(0);

	std::string tex_file = "assets/textures/hp.png";
	if (!texture.loadTexture(tex_file, 1)) {
		std::cout << "ERROR: Could not load texture: '" << tex_file << "'\n";
	}
}

HPIndicator::~HPIndicator() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void HPIndicator::Bind() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	texture.bind(0);
}
