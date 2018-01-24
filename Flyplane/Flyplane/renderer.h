#ifndef RENDERER_H
#define RENDERER_H
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <glm\glm.hpp>
#include "model.h"
#include "camera.h"
#include "heightmap.h"

class Renderer {
private:
	ShaderProgram shader;
	glm::mat4x4 matrix;
	Camera camera;
public:
	Renderer();
	~Renderer();
	void Render(Model &model);
	void Render(Heightmap &map);
};
#endif