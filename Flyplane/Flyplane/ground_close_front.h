#pragma once
#include "condition.h"
class Ground_Close_Front : public Condition {
public:
	Ground_Close_Front(float time, int security) : Condition() {
		this->time = time;
		this->security = security;
	}
	bool test(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest) {
		float speed = AI.component<FlightComponent>()->current_speed;
		glm::vec3 front = glm::normalize(glm::toMat3(AI.component<Transform>()->orientation) * glm::vec3(0.0, 0.0, 1.0));
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
		bool terrain_close = false;
		for (int i = 0; i < security; i++) {
			glm::vec3 p = AI.component<Transform>()->pos + ((front * speed * time * float(i)) / float(security));
			glm::vec3 pd = AI.component<Transform>()->pos + ((front * speed * time * float(i)) / float(security)) - (up * ((float(i) * time / 2.f) / float(security)));
			float hp = terrain.component<Terrain>()->hmptr->heightAt(p);
			float hpd = terrain.component<Terrain>()->hmptr->heightAt(pd);
			if (hp > p.y || pd.y < hpd) {
				terrain_close = true;
			}
		}
		return terrain_close;
	}
	float time;
	int security;
};