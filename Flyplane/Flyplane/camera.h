#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera {
private:
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	glm::mat4 projMatrix;

public:
	Camera();
	~Camera();
	void update();
	glm::mat4 getViewMatrix() const;
	const glm::mat4& getProjMatrix() const;
};