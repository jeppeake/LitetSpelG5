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
#include "speedindicator.h"
#include "hpindicator.h"


struct RenderObject {
	Model* model;
	Transform trans;
};

class Renderer {
private:
	ShaderProgram shader;
	ShaderProgram terrain_shader;
	ShaderProgram terrainShadow;
	ShaderProgram shadow;
	ShaderProgram guiShader;
	ShaderProgram enemyMarkerShader;
	ShaderProgram enemyArrowShader;
	ShaderProgram missileShader;
	ShaderProgram heightShader;
	ShaderProgram particleShader;

	GLuint quadVao, quadVbo;
	GLuint frameBuffer;
	GLuint depthTexture;

	GLuint terrainFrameBuffer;
	GLuint terrainDepthTexture;
	glm::mat4 planeShadowMatrix;
	glm::mat4 terrainShadowMatrix;

	glm::ivec2 shadowSize{ 4 * 1024, 4 * 1024 };
	glm::ivec2 terrainShadowSize{ 4 * 1024, 4*1024 };

	glm::mat4 debugMVP;
	Camera camera;
	std::vector<RenderObject> list;
	std::vector<RenderObject> listStatics;

	// old
	//std::vector<Heightmap*> mapList;

	glm::mat4 m;
	CrossHair crosshair;
	EnemyMarker markers;
	Cloud clouds;

	Texture hpbar;
	HPIndicator hpIndicator;
	glm::mat4 hpMatrix;

	Texture indicator;
	glm::mat4 heightMatrix;
	HeightIndicator heightIndicator;

	glm::mat4 speedMatrix;
	SpeedIndicator speedIndicator;

	int weaponAmmo;
	Model *missile = nullptr;
	glm::mat4 missileVPMatrix;
	glm::mat4 missileModelMatrix;
	Timer time;

	Heightmap* hm;
	std::vector<Patch> patches;
	std::vector<Patch> shadowPatches;

	Texture transparent;

	Texture scoreTexture;
	int score;
	Texture numbers[10];
	Texture x;
	int multiplier;

	bool isOutside = false;
	double outsideTime = -1;

	glm::vec3 sunDir;


	void Render(Model &model, Transform &trans);
	void Render(RenderObject& obj);

	// old
	//void RenderShadow(Model &model, Transform &trans);

	void RenderWeapon();
	void RenderCrosshair(glm::vec3 pos, glm::quat orientation);
	void RenderHPBar(float hp);
	void RenderHeightIndicator(float height);
	void RenderSpeedometer(float speed);
	void RenderOutsideMessage();
public:
	Renderer(const Renderer &other) = delete;
	static Renderer& getRenderer()
	{
		static Renderer s;
		return s;
	}
	Renderer();
	~Renderer();
	void addToList(Model* model, Transform trans, bool isStatic = false);
	void setTerrainPatches(const std::vector<Patch>& patches);
	void setTerrainPatchesShadow(const std::vector<Patch>& patches);
	void setHeightmap(Heightmap* hm) {
		this->hm = hm;
	}
	
	void RenderPlaneShadow();

	void RenderTerrainShadow();

	void RenderScene();
	void RenderGui(float hp, float height, float speed, glm::vec3 crosshairPos, glm::quat orientation);
	void RenderTransparent();
	void RenderClouds();
	void setWeaponModel(Model *mptr);
	void setAmmo(int ammo);
	Camera getCamera() &;
	void setCamera(const Camera& camera);
	void addMarker(glm::vec3 pos, float scale);
	void addMarker(glm::vec3 pos, glm::vec3 color, float scale);
	void renderTexture(const Texture& texture, const glm::mat4& matrix);
	void setScore(int points);
	void setMultiplier(int multiplier);

	void renderParticles(Particles* p);

	void setIsOutside(bool isOutside, double outsideTime = -2) {
		this->isOutside = isOutside;
		this->outsideTime = outsideTime;
	}


	glm::mat4 getTerrainShadowMatrix() {
		return this->terrainShadowMatrix;
	}

	//void setCrosshairPos(glm::vec3 pos);
	//glm::mat4& getCrosshairPos();
	//glm::quat orientation;
	// DEBUG
	void update(float dt);
};