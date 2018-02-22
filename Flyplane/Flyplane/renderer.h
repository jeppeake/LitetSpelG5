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
#include "particles.h"
#include "crosshair.h"
#include "enemymarker.h"
#include "cloud.h"
#include "timer.h"
#include "heightindicator.h"

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
	ShaderProgram missileShader;
	ShaderProgram heightShader;

	GLuint quadVao, quadVbo;
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
	Cloud clouds;

	Texture hpbar;
	Texture hpTexture;
	glm::mat4 hpMatrix;

	Texture indicator;
	glm::mat4 heightMatrix;
	HeightIndicator heightIndicator;

	int weaponAmmo;
	Model *missile = nullptr;
	glm::mat4 missileVPMatrix;
	glm::mat4 missileModelMatrix;
	Timer time;

	Timer globalTime;

	Heightmap* hm;
	std::vector<Patch> patches;


	void Render(Model &model, Transform &trans);
	void Render(RenderObject& obj);
	void RenderShadow(Model &model, Transform &trans);
	void RenderWeapon();
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
	void RenderClouds();
	void RenderHPBar(float hp);
	void RenderHeightIndicator(float height);
	void setWeaponModel(Model *mptr);
	void setAmmo(int ammo);
	Camera getCamera() &;
	void setCamera(const Camera& camera);
	void addMarker(glm::vec3 pos, float scale);
	void addMarker(glm::vec3 pos, glm::vec3 color, float scale);
	void renderTexture(const Texture& texture, const glm::mat4& matrix);
	void setCrosshairPos(glm::vec3 pos);
	glm::mat4& getCrosshairPos();
	glm::quat orientation;
	// DEBUG
	void update(float dt);
};