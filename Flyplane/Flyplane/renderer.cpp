#include "renderer.h"
#include "transform.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include <iostream>
using namespace std;

Renderer::Renderer() {
	glEnable(GL_TEXTURE_2D);
	this->shader.create("vertexShader.glsl", "fragmentShader.glsl");
	this->terrain_shader.create("terrainVertexShader.glsl","geometryShader.glsl", "terrainFragmentShader.glsl");
	this->shadow.create("shadowVertexShader.glsl", "shadowFragmentShader.glsl");
}

Renderer::~Renderer() {

}

void Renderer::Render(Model &model, Transform &trans) {

	glm::mat4 modelMatrix = glm::translate(trans.pos) * glm::toMat4(trans.orientation);

	this->shader.use();
	shader.uniform("texSampler", 0);
	model.texture.bind(0);
	this->tal += 0.005;
	this->shader.uniform("ViewProjMatrix", this->camera.getProjMatrix() * this->camera.getViewMatrix());
	for (int i = 0; i < model.model_meshes.size(); i++) {
		model.model_meshes[i].first->bind();
		this->shader.uniform("modelMatrix", modelMatrix);
		glDrawElements(GL_TRIANGLES, model.model_meshes[i].first->numIndices(), GL_UNSIGNED_INT, 0);
	}
}

void Renderer::Render(Heightmap &map) {
	this->terrain_shader.use();
	this->terrain_shader.uniform("ViewProjMatrix", this->camera.getProjMatrix() * this->camera.getViewMatrix());
	map.bind();
	glm::mat4 trans(1);
	trans = glm::translate(trans, map.pos);
	this->terrain_shader.uniform("modelMatrix", trans);
	glDrawElements(GL_TRIANGLES, (GLuint)map.indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::update(float dt)
{
	camera.update(dt);
}

