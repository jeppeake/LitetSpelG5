#include "fullscreenimage.h"

std::vector<float> vert{
	0,0,
	1,0,
	0,1,
	1,1
};

std::vector<unsigned int> ind{
	0, 1, 2,
	1, 3, 2
};

void FullscreenImage::load(std::string filename) {
	tex.loadTexture(filename);

	p.create("fcImagevertex.glsl", "fcimageFrag.glsl");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(GLuint), &ind[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vert.size(), &vert[0], GL_STATIC_DRAW);


	unsigned int stride = 2*sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);


	glBindVertexArray(0);
}

void FullscreenImage::bind() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glActiveTexture(GL_TEXTURE0);
	tex.bind(0);
}

void FullscreenImage::unbind() {
	glBindVertexArray(0);
}

void FullscreenImage::draw() {
	p.use();
	//p.uniform("position", vert);
	p.uniform("texSampler", 0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}