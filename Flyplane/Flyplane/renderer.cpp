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
	glEnable(GL_TEXTURE_2D);
	this->shader.create("vertexShader.glsl", "fragmentShader.glsl");
	this->terrain_shader.create("terrainVertexShader.glsl","geometryShader.glsl", "terrainFragmentShader.glsl");
	this->shadow.create("shadowVertexShader.glsl", "shadowFragmentShader.glsl");
	this->shader.uniform("shadowMap", 1);
	this->terrain_shader.uniform("shadowMap", 1);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_BORDER?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "framebuffer broken" << endl;

	glm::mat4 proj = glm::ortho<float>(-100000, 100000, -100000, 100000, -100000, 100000);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 m(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 0.1
	);
	this->shadowMatrix = m * proj * view;
	view = glm::lookAt(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(1, 0, 0));
	proj = glm::ortho<float>(-10000, 10000, -100, 100, -100, 100);
	debugMVP = proj * view;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::~Renderer() {

}

void Renderer::addToList(Model* model, Transform* trans) {
	list.push_back({ model, trans });
}

void Renderer::addToList(Heightmap* map) {
	mapList.push_back(map);
}

void Renderer::Render(Model &model, Transform &trans) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glm::mat4 modelMatrix = glm::translate(trans.pos) * glm::toMat4(trans.orientation);

	this->shader.use();

	shader.uniform("texSampler", 0);
	model.texture.bind(0);

	this->shader.uniform("ViewProjMatrix", this->camera.getProjMatrix() * this->camera.getViewMatrix());

	for (int i = 0; i < model.model_meshes.size(); i++) {
		model.model_meshes[i].first->bind();
		this->shader.uniform("modelMatrix", modelMatrix*model.model_meshes[i].second);
		shader.uniform("shadowMatrix", shadowMatrix * modelMatrix);
		glDrawElements(GL_TRIANGLES, model.model_meshes[i].first->numIndices(), GL_UNSIGNED_INT, 0);
	}
}


void Renderer::Render(RenderObject& obj) {
	Render(*obj.model, *obj.trans);
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
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);

	//Render shadow
	for (int i = 0; i < list.size(); i++) {
		glm::mat4 modelMatrix = glm::translate(list[i].trans->pos) * glm::toMat4(list[i].trans->orientation);

		for (int j = 0; j < list[i].model->model_meshes.size(); j++) {
			list[i].model->model_meshes[j].first->bind();
			shadow.uniform("MVP", shadowMatrix * modelMatrix * list[i].model->model_meshes[j].second);
			glDrawElements(GL_TRIANGLES, list[i].model->model_meshes[j].first->numIndices(), GL_UNSIGNED_INT, 0);
		}
	}


	//Render scene
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	auto s = Window::getWindow().size();
	glViewport(0, 0, s.x, s.y); //size of viewport?
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();
	shader.uniform("texSampler", 0);
	shader.uniform("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	shader.uniform("ViewProjMatrix", this->camera.getProjMatrix() * this->camera.getViewMatrix());
	//shader.uniform("ViewProjMatrix", debugMVP);

	
	for (int i = 0; i < list.size(); i++) {
		/*
		glm::mat4 modelMatrix = glm::translate(list[i].trans->pos) * glm::toMat4(list[i].trans->orientation);
		list[i].model->texture.bind(0);
		this->shader.uniform("modelMatrix", modelMatrix);
		shader.uniform("shadowMatrix", shadowMatrix * modelMatrix);

		for (int j = 0; j < list[i].model->model_meshes.size(); j++) {
			auto m = list[i].model->model_meshes[j];
			m.first->bind();
			this->shader.uniform("modelMatrix", modelMatrix*m.second);
			//shader.uniform("shadowMatrix", shadowMatrix * modelMatrix);
			glDrawElements(GL_TRIANGLES, list[i].model->model_meshes[j].first->numIndices(), GL_UNSIGNED_INT, 0);
		}
		*/
		Render(list[i]);
	}
	//Render terrain
	terrain_shader.use();
	terrain_shader.uniform("shadowMatrix", shadowMatrix);
	terrain_shader.uniform("ViewProjMatrix", this->camera.getProjMatrix() * this->camera.getViewMatrix());

	terrain_shader.uniform("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	for (int i = 0; i < mapList.size(); i++) {
		terrain_shader.uniform("texSampler", 0);
		glActiveTexture(GL_TEXTURE0);
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
}

void Renderer::update(float dt)
{

}

