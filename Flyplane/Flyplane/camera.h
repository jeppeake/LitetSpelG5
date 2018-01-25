#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "transform.h"

class Camera {
private:
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	glm::mat4 projMatrix;

	float yaw, pitch;

public:
	Camera();
	~Camera();
	//void update(float dt);
	glm::mat4 getViewMatrix() const;
	const glm::mat4& getProjMatrix() const;
	void setTransform(const Transform& t);
};