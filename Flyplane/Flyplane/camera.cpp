#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <glm\gtx\transform.hpp>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "window.h"
#include "input.h"

Camera::Camera() {
	this->projMatrix = glm::infinitePerspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f);
}

Camera::~Camera() {

}

/*
void Camera::update(float dt)
{
	// DEBUG FPS CAMERA
	auto dm = Input::mouseMov();
	pitch += dm.y * 0.005f;
	yaw -= dm.x * 0.005f;

	float hpi = glm::half_pi<float>() - 0.001;
	pitch = glm::clamp(pitch, -hpi, hpi);

	float speed = 15.f;
	if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
		speed = 50.f;

	

	glm::vec3 walk_dir;
	if (Input::isKeyDown(GLFW_KEY_W))
		walk_dir += glm::vec3(0, 0, 1);
	if (Input::isKeyDown(GLFW_KEY_S))
		walk_dir += glm::vec3(0, 0, -1);
	if (Input::isKeyDown(GLFW_KEY_A))
		walk_dir += glm::vec3(1, 0, 0);
	if (Input::isKeyDown(GLFW_KEY_D))
		walk_dir += glm::vec3(-1, 0, 0);

	glm::mat3 rot = glm::rotate(yaw, glm::vec3(0,1,0))*glm::rotate(pitch, glm::vec3(1, 0, 0));
	forward = normalize(rot * glm::vec3(0,0,1));

	walk_dir = rot * walk_dir;

	pos += walk_dir * dt * speed;
	if (Input::isKeyDown(GLFW_KEY_SPACE))
		pos.y += speed * dt;
	if (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL))
		pos.y -= speed * dt;

	auto size = Window::getWindow().size();
	this->projMatrix = glm::infinitePerspective(glm::radians(90.0f), size.x/size.y, 0.1f);

}
*/

glm::mat4 Camera::getViewMatrix() const
{
	return glm::inverse(glm::translate(transform.pos)*glm::toMat4(glm::rotate(transform.orientation, glm::pi<float>(), glm::vec3(0,1,0))));
}

const glm::mat4& Camera::getProjMatrix() const
{
	return this->projMatrix;
}

void Camera::setTransform(const Transform &t, float _fov)
{
	
	/*
	pos = t.pos;
	forward = glm::toMat3(t.orientation)*glm::vec3(0, 0, 1);
	up = glm::toMat3(t.orientation)*glm::vec3(0, 1, 0);
	*/
	transform = t;

	this->fov = _fov;

	// BAD
	auto size = Window::getWindow().size();
	this->projMatrix = glm::infinitePerspective(glm::radians(fov), size.x / size.y, 0.1f);

	//this->projMatrix = glm::perspective(glm::radians(fov), size.x / size.y, 0.1f, 10'000.f);

	glm::mat4 finiteProjMatrix = glm::perspective(glm::radians(fov), size.x / size.y, 1.f, 100.f);
	this->invViewProj = glm::inverse(finiteProjMatrix * this->getViewMatrix());
}

Transform Camera::getTransform() const
{
	return transform;
}

float Camera::getFov() const {
	return fov;
}

glm::mat4 Camera::getInverse() const {
	return invViewProj;
}


