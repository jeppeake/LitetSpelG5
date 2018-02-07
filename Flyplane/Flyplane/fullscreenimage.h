#pragma once
#include "texture.h"
#include "model.h"
#include "shader.h"

class FullscreenImage {
private:
	Texture tex;
	unsigned int width, height;
	GLuint ebo = 0;
	GLuint vbo = 0;
	GLuint vao = 0;
	ShaderProgram p;
public:
	FullscreenImage() {}
	void load(std::string filename);
	~FullscreenImage() {}
	void bind();
	void unbind();
	void draw();
};