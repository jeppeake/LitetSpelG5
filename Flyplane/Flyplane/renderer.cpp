#include "renderer.h"
#include "transform.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include <iostream>
#include "window.h"
#include "assetloader.h"
#include "input.h"
#include "globaltimer.h"
#include <sstream>
#include <iomanip>

using namespace std;

Renderer::Renderer() {
	this->shader.create("vertexShader.glsl", "fragmentShader.glsl");
	this->terrain_shader.create("terrainVertexShader.glsl","geometryShader.glsl", "terrainFragmentShader.glsl");
	this->shadow.create("shadowVertexShader.glsl", "terrainShadowFrag.glsl");
	this->shadowStatic.create("shadowVertexShader.glsl", "shadowFragmentShader.glsl");
	this->terrainShadow.create("terrainShadowVert.glsl", "terrainShadowFrag.glsl");
	this->guiShader.create("guiVertexSHader.glsl", "guiFragmentShader.glsl");
	this->enemyMarkerShader.create("enemymarkerVS.glsl","enemymarkerGS.glsl", "enemymarkerFS.glsl");
	this->enemyArrowShader.create("enemymarkerVS.glsl", "enemyarrowGS.glsl", "enemymarkerFS.glsl");
	this->missileShader.create("missileVS.glsl", "missileFS.glsl");
	this->heightShader.create("heightindicatorVS.glsl", "heightindicatorFS.glsl");
	this->particleShader.create("partVert.glsl", "partGeom.glsl", "partFrag.glsl");
	this->particleLineShader.create("partLineVert.glsl", "partFrag.glsl");

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowSize.x, shadowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "framebuffer broken" << endl;



	glGenFramebuffers(1, &terrainFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, terrainFrameBuffer);

	glGenTextures(1, &terrainDepthTexture);
	glBindTexture(GL_TEXTURE_2D, terrainDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, terrainShadowSize.x, terrainShadowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, terrainDepthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "framebuffer broken" << endl;



	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	m = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1
	);
	
	

	float vertexbuffer[] = {
		-1.0,  1.0, 0.0,
		 0.0,  1.0,

		 1.0,  1.0, 0.0,
		 1.0,  1.0,

		-1.0, -1.0, 0.0,
		 0.0,  0.0,

		 1.0, -1.0, 0.0,
		 1.0,  0.0
	};

	glGenVertexArrays(1, &quadVao);
	glGenBuffers(1, &quadVbo);
	glBindVertexArray(quadVao);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertexbuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glBindVertexArray(0);

	hpbar.loadTexture("assets/textures/hpbar.png");
	//hpTexture.loadTexture("assets/textures/hp.png", 1);
	hpMatrix = glm::translate(glm::vec3(-0.8, -0.8, 0)) * glm::scale(glm::vec3(0.15, 0.05, 1));

	indicator.loadTexture("assets/textures/indicator.png", 1);
	heightMatrix = glm::translate(glm::vec3(-0.8, -0.1, 0)) * glm::scale(glm::vec3(0.1, 0.5, 1));

	speedIndicator = new SpeedIndicator(18.0f, "assets/textures/speedbars.png");
	speedIndicator2 = new SpeedIndicator(15.0f, "assets/textures/speedbars2.png");
	speedMatrix = glm::translate(glm::vec3(0.8, -0.1, 0)) * glm::rotate(glm::pi<float>(), glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(0.1, 0.5, 1));

	missileVPMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 10.0f);
	missileModelMatrix = glm::rotate(3.14f / 4.0f, glm::vec3(0, 0, -1)) * glm::rotate(3.14f / 4.0f, glm::vec3(-1, 0, 0));

	flareReadyTexture.hint(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	flareReadyTexture.loadTexture("assets/Textures/flareicon.png", 1);
	
	aimAssistTexture.loadTexture("assets/Textures/aimassist.png", 1);
	smallDotTexture.loadTexture("assets/Textures/dot.png", 1);

	beam.loadTexture("assets/Textures/beam.png", 1);
	transparent.loadTexture("assets/Textures/transparent.png", 1);
	warning.loadTexture("assets/Textures/warning.png", 1);
	scoreTexture.loadTexture("assets/Textures/score.png", 1);
	plus.loadTexture("assets/Textures/+.png", 1);

	for (int i = 0; i < 10; i++) {
		numbers[i].loadTexture("assets/Textures/" + to_string(i) + ".png", 1);
	}
	for (int i = 0; i < 10; i++) {
		bNumbers[i].loadTexture("assets/Textures/b" + to_string(i) + ".png", 1);
	}
	x.loadTexture("assets/Textures/x.png", 1);
	ammoTexture.loadTexture("assets/Textures/ammo.png", 1);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, terrainFrameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	sunDir = normalize(glm::vec3(0, 1, 2));

	InitBullets();
}

Renderer::~Renderer() {
	delete speedIndicator;
	delete speedIndicator2;
}

void Renderer::addToList(Model* model, Transform trans, bool isStatic) {
	if (isStatic) {
		listStatics.push_back({ model, trans });
	} else {
		list.push_back({ model, trans });
	}
}



void Renderer::setTerrainPatches(const std::vector<Patch>& patches) {
	this->patches = patches;
}

void Renderer::setTerrainPatchesShadow(const std::vector<Patch>& patches) {
	this->shadowPatches = patches;
}

void Renderer::Render(Model &model, Transform &trans) {
	glm::mat4 modelMatrix = glm::translate(trans.pos) * glm::toMat4(trans.orientation) * glm::scale(trans.scale);
	model.texture.bind(0);

	for (int i = 0; i < model.model_meshes.size(); i++) {
		model.model_meshes[i].first->bind();
		this->shader.uniform("modelMatrix", modelMatrix*model.model_meshes[i].second);
		glDrawElements(GL_TRIANGLES, model.model_meshes[i].first->numIndices(), GL_UNSIGNED_INT, 0);
	}

	
}


void Renderer::Render(RenderObject& obj) {
	Render(*obj.model, obj.trans);
}

/*
// Old
void Renderer::RenderShadow(Model & model, Transform & trans) {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glm::mat4 modelMatrix = glm::translate(trans.pos) * glm::toMat4(trans.orientation);

	this->shadow.use();
	for (int i = 0; i < model.model_meshes.size(); i++) {
		model.model_meshes[i].first->bind();
		this->shader.uniform("MVP", shadowMatrix * modelMatrix);
		glDrawElements(GL_TRIANGLES, model.model_meshes[i].first->numIndices(), GL_UNSIGNED_INT, 0);
	}
}
*/


void Renderer::RenderWeapon() {
	auto s = Window::getWindow().size();
	glViewport(s.x - 300, 0, 150, 150);
	
	this->missileShader.use();
	missile->texture.bind(0);
	missile->model_meshes[0].first->bind();

	float scale = 1.0f / missile->getBoundingRadius();

	if (scale > 10000) {
		scale = 0.8;
	}

	this->missileShader.uniform("ViewProjMatrix", missileVPMatrix);
	float angle = time.elapsed();

	if (angle > 2 * 3.14) {
		time.restart();
	}
	this->missileShader.uniform("modelMatrix", missileModelMatrix * glm::rotate(angle, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(scale)));
	glDrawElements(GL_TRIANGLES, missile->model_meshes[0].first->numIndices(), GL_UNSIGNED_INT, 0);

	glViewport(0, 0, s.x, s.y);
	//AssetLoader::getLoader().getText()->drawText(std::to_string(weaponAmmo), glm::vec2(s.x - 300, 20), glm::vec3(1, 1, 0), 0.4);
	glViewport(s.x - 450, 25, 100, 30);
	renderTexture(ammoTexture, glm::mat4(1));
	string temp = to_string(weaponAmmo);

	for (int i = 0; i < temp.size(); i++) {
		glViewport(s.x - 340 + 14 * i, 30, 14, 20);
		renderTexture(bNumbers[temp[i] - '0'], glm::mat4(1));
	}
	glViewport(0, 0, s.x, s.y);
}

Timer t;


void Renderer::RenderPlaneShadow() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	shadow.use();
	glViewport(0, 0, shadowSize.x, shadowSize.y);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadow.uniform("texSampler", 0);
	//Render shadow
	for (int i = 0; i < list.size(); i++) {
		glm::mat4 modelMatrix = glm::translate(list[i].trans.pos) * glm::toMat4(list[i].trans.orientation) * glm::scale(list[i].trans.scale);
		list[i].model->texture.bind(0);
		for (int j = 0; j < list[i].model->model_meshes.size(); j++) {
			list[i].model->model_meshes[j].first->bind();
			shadow.uniform("MVP", planeShadowMatrix * modelMatrix * list[i].model->model_meshes[j].second);
			glDrawElements(GL_TRIANGLES, list[i].model->model_meshes[j].first->numIndices(), GL_UNSIGNED_INT, 0);
		}
	}
}

void Renderer::RenderTerrainShadow() {
	glBindFramebuffer(GL_FRAMEBUFFER, terrainFrameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowStatic.use();
	glViewport(0, 0, terrainShadowSize.x, terrainShadowSize.y);

	shadowStatic.uniform("texSampler", 0);
	for (int i = 0; i < listStatics.size(); i++) {
		auto& current = listStatics[i];
		glm::mat4 modelMatrix = glm::translate(current.trans.pos) * glm::toMat4(current.trans.orientation) * glm::scale(current.trans.scale);
		listStatics[i].model->texture.bind(0);

		for (int j = 0; j < current.model->model_meshes.size(); j++) {
			current.model->model_meshes[j].first->bind();
			shadowStatic.uniform("MVP", terrainShadowMatrix * modelMatrix * current.model->model_meshes[j].second);
			glDrawElements(GL_TRIANGLES, current.model->model_meshes[j].first->numIndices(), GL_UNSIGNED_INT, 0);
		}
	}


	terrainShadow.use();
	if (hm != NULL) {
		terrainShadow.uniform("time", (float)GlobalTimer::elapsed());
		terrainShadow.uniform("ViewProjMatrix", this->terrainShadowMatrix);
		hm->bind(terrainShadow);
		for (int i = 0; i < shadowPatches.size(); i++) {
			int indices = shadowPatches[i].indices;
			hm->bindIndices(indices);

			terrainShadow.uniform("offset", shadowPatches[i].offset);
			terrainShadow.uniform("patch_size", glm::vec2(shadowPatches[i].size));
			glDrawElements(GL_TRIANGLES, (GLuint)hm->indices[indices].size(), GL_UNSIGNED_INT, 0);
		}
	}
}

void Renderer::RenderTerrain() {
	glm::mat4 viewProjMatrix = this->camera.getProjMatrix() * this->camera.getViewMatrix();
	//Render terrain
	terrain_shader.use();
	terrain_shader.uniform("shadowMatrix", m * planeShadowMatrix);
	terrain_shader.uniform("terrainShadowMatrix", m * terrainShadowMatrix);
	terrain_shader.uniform("ViewProjMatrix", viewProjMatrix);
	terrain_shader.uniform("sunDir", sunDir);

	terrain_shader.uniform("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	terrain_shader.uniform("terrainShadowMap", 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, terrainDepthTexture);
	if (hm != NULL) {
		terrain_shader.uniform("time", (float)GlobalTimer::elapsed());
		terrain_shader.uniform("cameraPos", camera.getTransform().pos);
		hm->bind(terrain_shader);
		for (int i = 0; i < patches.size(); i++) {
			int indices = patches[i].indices;
			hm->bindIndices(indices);

			terrain_shader.uniform("offset", patches[i].offset);
			terrain_shader.uniform("patch_size", glm::vec2(patches[i].size));
			glDrawElements(GL_TRIANGLES, (GLuint)hm->indices[indices].size(), GL_UNSIGNED_INT, 0);
		}
	}
}

void Renderer::RenderObjects() {
	shader.use();
	shader.uniform("texSampler", 0);

	shader.uniform("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	shader.uniform("terrainShadowMap", 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, terrainDepthTexture);

	glm::mat4 viewProjMatrix = this->camera.getProjMatrix() * this->camera.getViewMatrix();
	shader.uniform("ViewProjMatrix", viewProjMatrix);
	shader.uniform("shadowMatrix", m * planeShadowMatrix);
	shader.uniform("terrainShadowMatrix", m * terrainShadowMatrix);
	shader.uniform("cameraPos", camera.getTransform().pos);
	shader.uniform("sunDir", sunDir);
	shader.uniform("drawShadows", (int)drawShadows);
	for (int i = 0; i < list.size(); i++) {
		Render(list[i]);
	}
	for (int i = 0; i < listStatics.size(); i++) {
		Render(listStatics[i]);
	}
}

void Renderer::RenderScene() {

	if (drawShadows) {
		//glCullFace(GL_FRONT);
		RenderPlaneShadow();
		//glCullFace(GL_BACK);
		RenderTerrainShadow();
	}


	//Render scene
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	auto s = Window::getWindow().size();
	glViewport(0, 0, s.x, s.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	RenderBullets();

	RenderTerrain();

	RenderObjects();

	RenderClouds();

	RenderParticles();

	RenderBeams();

	glm::mat4 viewProjMatrix = this->camera.getProjMatrix() * this->camera.getViewMatrix();

	//Render markers
	enemyMarkerShader.use();
	markers.bind();
	std::vector<Marker> p = markers.getMarkers();
	enemyMarkerShader.uniform("aspectMatrix", viewProjMatrix);
	enemyMarkerShader.uniform("cameraPos", this->camera.getTransform().pos);
	std::vector<Marker> arrows;
	//glm::mat4 matrix = glm::perspectiveFov(glm::radians(45.0f), 19200.0f, 1080.0f, 0.1f, 200000.0f) *	camera.getViewMatrix();

	for (int i = 0; i < p.size(); i++) {
		glm::vec4 temp = viewProjMatrix * glm::vec4(p[i].pos, 1);
		temp = temp / temp.w;

		if (temp.x < -1.0 || temp.x > 1.0 || temp.y < -1.0 || temp.y > 1.0 || temp.z >= 1.0) {
			arrows.push_back({p[i].pos, p[i].color, p[i].scale });
		}
		else {
			enemyMarkerShader.uniform("transform", p[i].pos);
			enemyMarkerShader.uniform("scale", p[i].scale);
			enemyMarkerShader.uniform("color", p[i].color);
			glDrawArrays(GL_POINTS, 0, 1);
		}
	}
	//Render arrows
	enemyArrowShader.use();
	enemyArrowShader.uniform("aspectMatrix", viewProjMatrix);
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < arrows.size(); i++) {
		enemyArrowShader.uniform("transform", arrows[i].pos);
		//enemyMarkerShader.uniform("scale", p[i].scale);
		enemyArrowShader.uniform("color", arrows[i].color);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	glEnable(GL_CULL_FACE);
	markers.clear();
	list.clear();
	listStatics.clear();

	// old
	//mapList.clear();
}

void Renderer::RenderBeams() {
	std::sort(beams.begin(), beams.end(), [&](auto& a, auto& b) {
		float aDist = length(a.first - camera.getTransform().pos);
		float bDist = length(b.first - camera.getTransform().pos);
		return aDist > bDist;
	});

	glm::mat4 viewProjMatrix = this->camera.getProjMatrix() * this->camera.getViewMatrix();

	for (auto& b : beams) {
		glm::vec3 dir = b.first - camera.getTransform().pos;
		float distance = length(dir);
		float edge1 = 1500.f;
		float edge2 = 3000.f;
		float alpha = glm::smoothstep(edge1, edge2, distance);
		if (alpha > 1.f / 255.f) {
			glm::vec3 pos = b.first + 20.f*normalize(dir);
			if (hm)
				pos.y = hm->heightAt(pos) - 100;
			dir.y = 0;
			float angle = -acos(dot(normalize(dir), glm::vec3(0, 0, 1)));
			glm::vec3 axis = cross(normalize(dir), glm::vec3(0, 0, 1));
			glm::mat4 transform = glm::translate(pos) * glm::rotate(angle, axis) * glm::scale(glm::vec3(50, 3000, 50));
			renderTexture(beam, viewProjMatrix*transform, glm::vec4(b.second, alpha));
		}
	}

	beams.clear();
}

void Renderer::RenderGui(float hp, float height, float speed, glm::vec3 crosshairPos, glm::quat orientation) {
	auto s = Window::getWindow().size();
	string temp;

	if (!isDead) {
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
	
		RenderCrosshair(crosshairPos, orientation);
		RenderHPBar(hp);
		RenderHeightIndicator(height);
		RenderSpeedometer(speed);
	
		if (points > 0) {
			glViewport(200, s.y - 40, 20, 30);
			renderTexture(plus, glm::mat4(1));
			temp = to_string(points);
	
			for (int i = 0; i < temp.size(); i++) {
				glViewport(220 + 20 * i, s.y - 40, 20, 30);
				renderTexture(numbers[temp[i] - '0'], glm::mat4(1));
			}
		}
	
		glViewport(10, s.y - 60, 16, 24);
		renderTexture(x, glm::mat4(1));
	
		temp = to_string(multiplier);
		for (int i = 0; i < temp.size(); i++) {
			glViewport(26 + 16 * i, s.y - 60, 16, 24);
			renderTexture(numbers[temp[i] - '0'], glm::mat4(1));
		}
		if (isTargeted) {
			glViewport(s.x * 0.5 - 150, s.y * 0.8, 300, 50);
			renderTexture(warning, glm::mat4(1));
		}
	
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	
		//Render weapon
		if (weaponAmmo)
			RenderWeapon();
	
		if (isOutside)
			RenderOutsideMessage();
	}
	glViewport(0, s.y - 40, 100, 40);
	renderTexture(scoreTexture, glm::mat4(1));

	temp = to_string(score);

	for (int i = 0; i < temp.size(); i++) {
		glViewport(105 + 16 * i, s.y - 32, 16, 24);
		renderTexture(numbers[temp[i] - '0'], glm::mat4(1));
	}
	glViewport(0, 0, s.x, s.y);
}

void Renderer::RenderTransparent() {
	glDisable(GL_DEPTH_TEST);
	renderTexture(transparent, glm::mat4());
	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderCrosshair(glm::vec3 pos, glm::quat orientation) {
	guiShader.use();
	glm::mat4 position = camera.getProjMatrix() * camera.getViewMatrix() * glm::translate(pos);
	guiShader.uniform("matrix", position * glm::mat4(orientation) * glm::rotate(3.14f / 4, glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(50, 50, 1)));
	guiShader.uniform("texSampler", 0);
	crosshair.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::RenderClouds() {
	if(drawClouds)
		for (int i = 0; i < clouds.transform.size(); i++) {
			renderTexture(clouds.texture, camera.getProjMatrix() * camera.getViewMatrix() * clouds.transform[i]);
		}
}

void Renderer::RenderFlareReady() {
	glDisable(GL_DEPTH_TEST);

	auto ws = Window::getWindow().size();
	float aspect = ws.x / ws.y;


	glm::mat4 transform = glm::translate(glm::vec3(-0.5, -0.8, 0)) * glm::scale(0.03f*glm::vec3(4.02f, aspect*0.92f, 1));
	renderTexture(flareReadyTexture, transform);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderAimAssist(glm::vec3 markerPos, glm::vec3 targetPos) {
	auto screenPosH = camera.getProjMatrix() * camera.getViewMatrix() * glm::vec4(markerPos, 1);
	
	if (screenPosH.w < 0)
		return;

	auto screenMPos = glm::vec3(screenPosH) / screenPosH.w;

	screenPosH = camera.getProjMatrix() * camera.getViewMatrix() * glm::vec4(targetPos, 1);

	auto screenTPos = glm::vec3(screenPosH) / screenPosH.w;

	auto ws = Window::getWindow().size();
	float aspect = ws.x / ws.y;

	//std::cout << "screenpos: " << screenPos.x << ", " << screenPos.y << "\n";

	auto transform = glm::translate(glm::vec3(screenMPos.x, screenMPos.y, 0)) * glm::scale(0.006f*glm::vec3(1, aspect*1, 1));;
	renderTexture(aimAssistTexture, transform);

	float numDots = 3;
	for (float i = 1; i < numDots+1; i++) {
		glm::vec3 pos = screenMPos * i / (numDots+1) + screenTPos * (numDots+1 - i) / (numDots+1);
		pos.z = 0;
		transform = glm::translate(pos) * glm::scale(0.002f*glm::vec3(1, aspect * 1, 1));;
		renderTexture(smallDotTexture, transform);
	}
}

void Renderer::RenderHPBar(float hp) {
	renderTexture(hpbar, hpMatrix);
	heightShader.use();
	if (hp < 0)
		hp = 0;
	heightShader.uniform("value", glm::vec2((1 - hp) / 2.0f, 0));
	hpIndicator.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::RenderHeightIndicator(float height) {
	float realHeight = (height);
	renderTexture(indicator, heightMatrix);
	heightShader.use();
	heightShader.uniform("value", glm::vec2(0, realHeight / 23000.0f));
	heightIndicator.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	auto s = Window::getWindow().size();
	AssetLoader::getLoader().getText()->drawText(std::to_string((int)realHeight), glm::vec2(143 * s.x / 1280, 318 * s.y / 720), glm::vec3(0, 1, 0), 0.3 * s.y / 720 );
}

void Renderer::RenderSpeedometer(float speed) {
	renderTexture(indicator, speedMatrix);
	float realSpeed = speed * 3.6;
	heightShader.use();
	if (realSpeed < 1350) {
		heightShader.uniform("value", glm::vec2(0, realSpeed / 1800.0f));
		speedIndicator->Bind();
	}
	else {
		int speed = realSpeed - 1350.0f;
		if (speed > 1200)
			speed = 1200;
		heightShader.uniform("value", glm::vec2(0, speed / 1500.0f));
		speedIndicator2->Bind();
	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	auto s = Window::getWindow().size();
	AssetLoader::getLoader().getText()->drawText(std::to_string((int)realSpeed), glm::vec2(1100 * s.x / 1280, 318 * s.y / 720), glm::vec3(0, 1, 0), 0.3 * s.y / 720);
}

void Renderer::RenderOutsideMessage() {
	glDisable(GL_DEPTH_TEST);


	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << outsideTime;
	string boi = "8.3";
	RenderTransparent();
	auto s = Window::getWindow().size();
	double scalething = 20;
	double scalething2 = 20;
	AssetLoader::getLoader().getBigtext()->drawText("Return to the combat area!", glm::vec2(s.x/2 - scalething*13 * s.y / 720.0, s.y*0.6), glm::vec3(1, 1, 1), 1.0 * s.y / 720.0);
	AssetLoader::getLoader().getText()->drawText(ss.str(), glm::vec2(s.x/2 - scalething2*boi.length()/2 * s.y / 720.0, s.y*0.52), glm::vec3(1, 1, 1), 1.0 * s.y / 720.0);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::InitBullets() {
	bulletShader.create("bulletVert.glsl", "bulletGeom.glsl", "bulletFrag.glsl");

	glGenBuffers(1, &bulletPosVBO);
	glGenBuffers(1, &bulletQuatVBO);

	glGenVertexArrays(1, &bulletVAO);
	glBindVertexArray(bulletVAO);

	glBindBuffer(GL_ARRAY_BUFFER, bulletPosVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, bulletQuatVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::RenderBullets() {
	if (bulletPositions.size() > 0) {
	GLint access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	
	bulletShader.use();

	glBindBuffer(GL_ARRAY_BUFFER, bulletPosVBO);
	glBufferData(GL_ARRAY_BUFFER, bulletPositions.size() * sizeof(glm::vec3), &bulletPositions[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, bulletQuatVBO);
	glBufferData(GL_ARRAY_BUFFER, bulletOrientations.size() * sizeof(glm::vec4), &bulletOrientations[0], GL_DYNAMIC_DRAW);

	glm::mat4 viewProjMatrix = this->camera.getProjMatrix() * this->camera.getViewMatrix();
	bulletShader.uniform("ViewProjMatrix", viewProjMatrix);

	glEnable(GL_LINE_SMOOTH);
	float width = 2.0f;
	glLineWidth(width);
	glBindVertexArray(bulletVAO);
	glDrawArrays(GL_POINTS, 0, bulletPositions.size());
	glBindVertexArray(0);

	bulletPositions.clear();
	bulletOrientations.clear();
	}
}

void Renderer::setWeaponModel(Model * mptr) {
	missile = mptr;
}

void Renderer::setAmmo(int ammo) {
	weaponAmmo = ammo;
}

Camera Renderer::getCamera() &
{
	return camera;
}
void Renderer::setCamera(const Camera & camera)
{
	this->camera = camera;
	auto t = camera.getTransform();
	auto pos = t.pos;
	
	
	float halfSize = 50.f;
	glm::mat4 proj = glm::ortho<float>(-halfSize, halfSize, -halfSize, halfSize, 0.f, 500.f);
	glm::mat4 view = glm::lookAt(pos + 200.f*sunDir, pos, glm::vec3(0, 1, 0));

	this->planeShadowMatrix = proj * view;


	
	glm::vec2 minmax(0, 100);
	if (hm) {
		minmax = hm->getMinMaxHeights();
		
	}
	//std::cout << "minmax: " << minmax.x << ", " << minmax.y << "\n";
	//std::cout << "pos: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";

	halfSize = 4500.f;

	//std::cout << "halfSize: " << halfSize << "\n";


	float xHalfSize = halfSize;
	float yHalfSize = halfSize * sunDir.y;

	float h = (minmax.y - minmax.x)/sunDir.y;
	float a = glm::acos(sunDir.y);
	float extra = yHalfSize / glm::tan(glm::half_pi<float>() - a);

	


	//std::cout << "h:     " << h << "\n";
	//std::cout << "extra: " << extra << "\n";

	glm::vec3 offset = t.orientation * glm::vec3(0, 0, 0.3f*halfSize);
	offset.y = 0;

	float pixelSize = 2.f*halfSize / terrainShadowSize.y;
	float zjump = pixelSize / sunDir.y;
	pos = glm::vec3(pos.x, minmax.x, pos.z) + offset;
	pos.x = glm::floor(pos.x / pixelSize)*pixelSize;
	pos.z = glm::floor(pos.z / zjump)*zjump;

	proj = glm::ortho<float>(-xHalfSize, xHalfSize, -yHalfSize, yHalfSize, - h - extra, extra);
	view = glm::lookAt(pos + sunDir, pos, glm::vec3(0, 1, 0));
	
	this->terrainShadowMatrix = proj * view;
}

void Renderer::addMarker(glm::vec3 pos, float scale) {
	markers.addPosition(pos, scale);
}

void Renderer::addMarker(glm::vec3 pos, glm::vec3 color, float scale)
{
	markers.addPosition(pos, color, scale);
}

void Renderer::addBeam(glm::vec3 pos, glm::vec3 color) {
	beams.emplace_back(pos, color);
}

void Renderer::renderTexture(const Texture& texture, const glm::mat4& matrix, glm::vec4 colorFilter) {
	guiShader.use();
	guiShader.uniform("colorFilter", colorFilter);
	guiShader.uniform("matrix", matrix);
	guiShader.uniform("texSampler", 0);

	glBindVertexArray(quadVao);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
	texture.bind(0);

	glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
}

void Renderer::setScore(int points) {
	score = points;
}

void Renderer::setMultiplier(int multiplier) {
	this->multiplier = multiplier;
}

void Renderer::addParticles(Particles * p) {
	particles.push_back(p);
}

void Renderer::RenderParticles() {

	std::sort(particles.begin(), particles.end(), [](Particles* a, Particles* b) {
		return a->params.distFromCam > b->params.distFromCam;
	});

	for (auto p : particles) {
		if (p->type == WING_TRAIL) {
			glDisable(GL_LINE_SMOOTH);
			float width = 1.0f;
			glLineWidth(width);
			p->render(particleLineShader, GL_LINES);
		}
		else {
			p->render(particleShader, GL_POINTS);
		}
	}

	particles.clear();
}
void Renderer::setPoints(int points) {
	this->points = points;
}

/*void Renderer::setCrosshairPos(glm::vec3 pos) {
	crosshair.setMatrix(glm::translate(pos));
}

glm::mat4& Renderer::getCrosshairPos()
{
	return crosshair.getMatrix();
}*/

void Renderer::update(float dt)
{
	//sunDir = glm::rotate(glm::quat(), 0.1f*dt, glm::vec3(0, 0, 1)) * sunDir;

	if (Input::isKeyPressed(GLFW_KEY_F8)) {
		this->shader.reload();
		this->shadow.reload();
		this->shadowStatic.reload();
		this->terrain_shader.reload();
		this->bulletShader.reload();
	}
}

