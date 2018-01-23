#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	this->pos = glm::vec3(0, 20, 0);
	this->forward = glm::vec3(1, 0, 0);
	this->up = glm::vec3(0, 1, 0);
	this->projMatrix = glm::infinitePerspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f);
}

Camera::~Camera() {

}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(this->pos, this->pos + this->forward, this->up);
}

 const glm::mat4& Camera::getProjMatrix() const
{
	return this->projMatrix;
}

