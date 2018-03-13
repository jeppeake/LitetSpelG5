#include "speedindicator.h"
#include <iostream>

SpeedIndicator::SpeedIndicator(float size, std::string tex_file) {
	vertexdata = new float[20] { 0.787f, -0.6f, 0.0f, 0.0f, 0.0f,
		0.9f, -0.6f, 0.0f, 1.0f, 0.0f,
		0.787f,  0.4f, 0.0f, 0.0f, 3.0f / size,
		0.9f,  0.4f, 0.0f, 1.0f, 3.0f / size };
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

	//std::string tex_file = "assets/textures/speedbars.png";
	if (!texture.loadTexture(tex_file, 1)) {
		std::cout << "ERROR: Could not load texture: '" << tex_file << "'\n";
	}
}

SpeedIndicator::~SpeedIndicator() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void SpeedIndicator::Bind() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	texture.bind(0);
}
