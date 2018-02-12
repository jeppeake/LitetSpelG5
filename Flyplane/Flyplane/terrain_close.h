#pragma once
#include "condition.h"
class Terrain_Close : public Condition {
public:
	Terrain_Close(float time, int security, float rotSpeed) : Condition() {
		this->time = time;
		this->security = security;
		this->rotSpeed = rotSpeed;
	}
	bool test(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest) {
		float speed = AI.component<FlightComponent>()->current_speed;
		glm::vec3 front = glm::normalize(glm::toMat3(AI.component<Transform>()->orientation) * glm::vec3(0.0, 0.0, 1.0));
		glm::vec3 bottom = glm::normalize(glm::toMat3(AI.component<Transform>()->orientation) * glm::vec3(0.0, -1.0, 0.0));
		glm::vec3 rot_axle = glm::normalize(front + bottom);
		glm::vec3 check_vector = glm::rotate(front, angle, rot_axle);
		angle += rotSpeed;

		bool terrain_close = false;
		for (int i = 0; i < security; i++) {
			glm::vec3 p = AI.component<Transform>()->pos + ((check_vector * speed * time * float(i)) / float(security));
			float height = terrain.component<Terrain>()->hmptr->heightAt(p);
			if (height > p.y) {
				terrain_close = true;
			}
		}
		return terrain_close;
	}
	float angle = 0.0;
	float time, rotSpeed;
	int security;
};