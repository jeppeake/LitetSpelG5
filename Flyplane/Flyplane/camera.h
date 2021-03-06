#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "transform.h"

class Camera {
private:
	Transform transform;
	//glm::vec3 forward;
	//glm::vec3 up;
	glm::mat4 projMatrix;

	glm::mat4 invViewProj;

	float fov = 85.f;
	//float yaw, pitch;
public:
	Camera();
	~Camera();
	//void update(float dt);
	glm::mat4 getViewMatrix() const;
	const glm::mat4& getProjMatrix() const;
	void setTransform(const Transform& t, float fov = 85.f);
	Transform getTransform() const;

	float getFov() const;

	glm::mat4 getInverse() const;
};