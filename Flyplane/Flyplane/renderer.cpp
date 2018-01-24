#include "renderer.h"
#include <fstream>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>

using namespace std;

Renderer::Renderer() {
	this->shader.create("vertexShader.glsl", "fragmentShader.glsl");
	this->shader.use();
}

Renderer::~Renderer() {

}

void Renderer::Render(Model &model) {
	shader.uniform("Sampler", 17);
	model.texture.bind(17);
	this->tal += 0.0005;
	this->shader.uniform("ViewProjMatrix", this->camera.getProjMatrix() * this->camera.getViewMatrix());
	for (int i = 0; i < model.model_meshes.size(); i++) {
		model.model_meshes[i].first->bind();
		this->shader.uniform("modelMatrix", glm::rotate(model.model_meshes[i].second, this->tal, glm::vec3(0, 1, 0)));
		glDrawElements(GL_TRIANGLES, model.model_meshes[i].first->numIndices(), GL_UNSIGNED_INT, 0);
	}
}
