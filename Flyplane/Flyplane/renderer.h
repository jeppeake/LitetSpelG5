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
#include "crosshair.h"
#include "enemymarker.h"

struct RenderObject {
	Model* model;
	Transform trans;
};

class Renderer {
private:
	ShaderProgram shader;
	ShaderProgram terrain_shader;
	ShaderProgram shadow;
	ShaderProgram guiShader;
	ShaderProgram enemyMarkerShader;

	GLuint frameBuffer;
	GLuint depthTexture;
	glm::mat4 shadowMatrix;
	glm::mat4 debugMVP;
	Camera camera;
	std::vector<RenderObject> list;
	std::vector<Heightmap*> mapList;
	glm::mat4 m;
	CrossHair crosshair;
	EnemyMarker markers;

	Heightmap* hm;
	std::vector<Patch> patches;


	void Render(Model &model, Transform &trans);
	void Render(RenderObject& obj);
	void Render(Heightmap &map);
	void RenderShadow(Model &model, Transform &trans);
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
	void addToList(const std::vector<Patch>& patches);
	void setHeightmap(Heightmap* hm) {
		this->hm = hm;
	}
	
	void RenderScene();
	void RenderCrosshair();
	Camera getCamera() &;
	void setCamera(const Camera& camera);
	void addMarker(glm::vec3 pos, float scale);
	void setCrosshairPos(glm::vec3 pos);
	glm::mat4& getCrosshairPos();
	glm::quat orientation;
	// DEBUG
	void update(float dt);
};