#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

#include <glm\gtx\transform.hpp>
#include <GLFW\glfw3.h>
#include "input.h"

Camera::Camera() {
	this->pos = glm::vec3(0, 0, 20);
	this->forward = glm::vec3(1, 0, 0);
	this->up = glm::vec3(0, 1, 0);
	this->projMatrix = glm::infinitePerspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f);
	this->yaw = 0;
	this->pitch = 0;

}

Camera::~Camera() {

}

void Camera::update(float dt)
{
	// DEBUG FPS CAMERA

	Input::


	glm::vec3 walk_dir;
	if (Input::isKeyDown(GLFW_KEY_W))
		walk_dir += glm::vec3(0, 0, 1);
	if (Input::isKeyDown(GLFW_KEY_S))
		walk_dir += glm::vec3(0, 0, -1);
	if (Input::isKeyDown(GLFW_KEY_A))
		walk_dir += glm::vec3(1, 0, 0);
	if (Input::isKeyDown(GLFW_KEY_D))
		walk_dir += glm::vec3(-1, 0, 0);
	glm::mat3 rot = glm::rotate(yaw, glm::vec3(0,1,0))*glm::rotate(pitch, glm::vec3(0, 0, 1));
	forward = normalize(rot * walk_dir);


	pos += forward * dt * 1.f;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(this->pos, this->pos + this->forward, this->up);
}

 const glm::mat4& Camera::getProjMatrix() const
{
	return this->projMatrix;
}

