#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <glm\glm.hpp>
#include "model.h"
#include "camera.h"
#include "heightmap.h"
#include "transform.h"
#include "text.hpp"
#include "particlesystem.h"
struct RenderObject {
	Model* model;
	Transform trans;
};

class Renderer {
private:
	ShaderProgram shader;
	ShaderProgram terrain_shader;
	ShaderProgram shadow;
	GLuint frameBuffer;
	GLuint depthTexture;
	glm::mat4 shadowMatrix;
	glm::mat4 debugMVP;
	Camera camera;
	std::vector<RenderObject> list;
	std::vector<Heightmap*> mapList;
	glm::mat4 m;
public:
	Renderer(const Renderer &other) = delete;
	static Renderer& getRenderer()
	{
		static Renderer s;
		return s;
	}
	Renderer();
	~Renderer();
	void addToList(Model* model, Transform trans);
	void addToList(Heightmap* map);
	void Render(Model &model, Transform &trans);
	void Render(RenderObject& obj);
	void Render(Heightmap &map);
	void RenderShadow(Model &model, Transform &trans);
	void RenderScene();
	Camera getCamera() &;
	void setCamera(const Camera& camera);
	// DEBUG
	void update(float dt);
};