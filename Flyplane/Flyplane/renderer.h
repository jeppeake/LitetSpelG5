#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <glm\glm.hpp>
#include "model.h"
#include "camera.h"
#include "heightmap.h"
#include "transform.h"

class Renderer {
private:
	ShaderProgram shader;
	ShaderProgram terrain_shader;
	ShaderProgram shadow;
	Camera camera;
	float tal = 0;
public:
	Renderer(const Renderer &other) = delete;
	static Renderer& getRenderer()
	{
		static Renderer s;
		return s;
	}
	Renderer();
	~Renderer();
	void Render(Model &model, Transform &trans);
	void Render(Heightmap &map);

	void setCamera(const Camera& camera);
	// DEBUG
	void update(float dt);
};