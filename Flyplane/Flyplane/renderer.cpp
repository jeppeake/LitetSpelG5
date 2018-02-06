#include "renderer.h"
#include "transform.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include <iostream>
#include "window.h"
using namespace std;

Renderer::Renderer() {
	this->shader.create("vertexShader.glsl", "fragmentShader.glsl");
	this->terrain_shader.create("terrainVertexShader.glsl","geometryShader.glsl", "terrainFragmentShader.glsl");
	this->shadow.create("shadowVertexShader.glsl", "shadowFragmentShader.glsl");

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4*1024, 4*1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "framebuffer broken" << endl;

	
	m = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1
	);
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::~Renderer() {

}

void Renderer::addToList(Model* model, Transform trans) {
	list.push_back({ model, trans });
}

void Renderer::addToList(Heightmap* map) {
	mapList.push_back(map);
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


void Renderer::Render(Heightmap &map) {
	this->terrain_shader.use();
	this->terrain_shader.uniform("ViewProjMatrix", this->camera.getProjMatrix() * this->camera.getViewMatrix());
	terrain_shader.uniform("shadowMatrix", shadowMatrix);
	map.bind();
	glm::mat4 trans = glm::translate(map.pos);
	this->terrain_shader.uniform("modelMatrix", trans);
	glDrawElements(GL_TRIANGLES, (GLuint)map.indices.size(), GL_UNSIGNED_INT, 0);
}

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

void Renderer::RenderScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	shadow.use();
	glViewport(0, 0, 4*1024, 4*1024);
	glClear(GL_DEPTH_BUFFER_BIT);

	//Render shadow
	for (int i = 0; i < list.size(); i++) {
		glm::mat4 modelMatrix = glm::translate(list[i].trans.pos) * glm::toMat4(list[i].trans.orientation) * glm::scale(list[i].trans.scale);

		for (int j = 0; j < list[i].model->model_meshes.size(); j++) {
			list[i].model->model_meshes[j].first->bind();
			shadow.uniform("MVP", shadowMatrix * modelMatrix * list[i].model->model_meshes[j].second);
			glDrawElements(GL_TRIANGLES, list[i].model->model_meshes[j].first->numIndices(), GL_UNSIGNED_INT, 0);
		}
	}
	for (int i = 0; i < mapList.size(); i++) {
		mapList[i]->bind();
		glm::mat4 trans = glm::translate(mapList[i]->pos);
		this->shadow.uniform("MVP", shadowMatrix*trans);
		glDrawElements(GL_TRIANGLES, (GLuint)mapList[i]->indices.size(), GL_UNSIGNED_INT, 0);
	}


	//Render scene
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	auto s = Window::getWindow().size();
	glViewport(0, 0, s.x, s.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();
	shader.uniform("texSampler", 0);
	shader.uniform("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	shader.uniform("ViewProjMatrix", this->camera.getProjMatrix() * this->camera.getViewMatrix());
	shader.uniform("shadowMatrix", m * shadowMatrix);
	
	for (int i = 0; i < list.size(); i++) {
		Render(list[i]);
	}
	//Render terrain
	terrain_shader.use();
	terrain_shader.uniform("shadowMatrix", m * shadowMatrix);
	//terrain_shader.uniform("shadowMatrix", shadowMatrix);
	terrain_shader.uniform("ViewProjMatrix", this->camera.getProjMatrix() * this->camera.getViewMatrix());

	terrain_shader.uniform("texSampler", 0);
	terrain_shader.uniform("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	for (int i = 0; i < mapList.size(); i++) {
		mapList[i]->bind();
		glm::mat4 trans = glm::translate(mapList[i]->pos);
		this->terrain_shader.uniform("modelMatrix", trans);
		glDrawElements(GL_TRIANGLES, (GLuint)mapList[i]->indices.size(), GL_UNSIGNED_INT, 0);
	}
	list.clear();
	mapList.clear();
}

void Renderer::setCamera(const Camera & camera)
{
	this->camera = camera;
	auto pos = camera.getTransform().pos;
	pos += glm::toMat3(camera.getTransform().orientation)*glm::vec3(0,0, 1000.f);
	glm::mat4 proj = glm::ortho<float>(-2000.f, 2000.f, -2000.f, 2000.f, 0.f, 4000.f);
	glm::mat4 view = glm::lookAt(glm::vec3(pos.x, 2000.0f, 2000.0f + pos.z), glm::vec3(pos.x, 0, pos.z), glm::vec3(0, 1, 0));
	this->shadowMatrix = proj * view;
}

void Renderer::update(float dt)
{

}

