#pragma once
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
	ShaderProgram terrain_shader;
	Camera camera;
	float tal = 0;
public:
	Renderer();
	~Renderer();
	void Render(Model &model);
	void Render(Heightmap &map);


	// DEBUG
	void update(float dt);
};