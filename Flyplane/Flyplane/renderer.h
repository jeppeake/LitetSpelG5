#ifndef RENDERER_H
#define RENDERER_H
#include <GL\glew.h>
#include <GLFW/glfw3.h>

class Renderer {
private:
	GLuint vertex_buffer;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;
	GLint vertex_pos;
	GLint vertex_tex;
public:
	Renderer();
	~Renderer();
	void Render();
};
#endif