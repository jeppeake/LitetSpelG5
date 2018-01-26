#pragma once


#include <entityx\entityx.h>
#include <entityx\Entity.h>

#include <glm/gtc/quaternion.hpp>
#include "renderer.h"
#include "modelcomponent.h"
#include "transform.h"
#include "playercomponent.h"
#include "terraincomponent.h"
#include "equipment.h"

using namespace entityx;

struct RenderSystem : public System<RenderSystem> {

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<Transform> transform;
		for (Entity entity : es.entities_with_components(player, transform)) {
			player = entity.component<PlayerComponent>();
			transform = entity.component<Transform>();

			Transform cam = *transform.get();
			glm::vec3 offset(0, 4, -8);
			offset = glm::toMat3(cam.orientation)*offset;
			cam.pos += offset;

			Transform p_cam = player->camera.getTransform();

			double f_pos = 0.001;
			double f_orien = 0.001;
			p_cam.pos = glm::mix(p_cam.pos, cam.pos, float(1.0 - glm::pow(f_pos, dt)));
			p_cam.orientation = glm::mix(p_cam.orientation, cam.orientation, float(1.0 - glm::pow(f_orien, dt)));

			std::cout << p_cam.pos.y << " | " << cam.pos.y << "\n";

			player->camera.setTransform(p_cam);

			Renderer::getRenderer().setCamera(player->camera);
		}




		ComponentHandle<ModelComponent> model;
		for (Entity entity : es.entities_with_components(model, transform)) {
			model = entity.component<ModelComponent>();
			transform = entity.component<Transform>();
			Renderer::getRenderer().Render(*model->mptr, *transform.get());
		}

		ComponentHandle<Terrain> terrain;
		for (Entity entity : es.entities_with_components(terrain)) {
			terrain = entity.component<Terrain>();
			Renderer::getRenderer().Render(*terrain->hmptr);
		}

		ComponentHandle<Equipment> equip;
		for (Entity entity : es.entities_with_components(equip, transform)) {
			equip = entity.component<Equipment>();
			transform = entity.component<Transform>();
			for (int i = 0; i < equip->slots.size(); i++) {
				Transform newTrans;
				newTrans.orientation = transform->orientation;
				newTrans.pos = transform->pos + glm::toMat3(transform->orientation) * equip->slots[i].offset;

				Renderer::getRenderer().Render(*equip->slots[i].model, newTrans);
			}
		}
	}

};