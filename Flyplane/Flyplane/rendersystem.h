#pragma once


#include <entityx\entityx.h>
#include <entityx\Entity.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "renderer.h"
#include "modelcomponent.h"
#include "transform.h"
#include "playercomponent.h"
#include "terraincomponent.h"
#include "projectilecomponent.h"
#include "equipment.h"
#include "physics.h"
#include "particlesystem.h"
#include "radar.h"

using namespace entityx;

struct RenderSystem : public System<RenderSystem> {
	ParticleSystem *S;
	RenderSystem()
	{
		S = new ParticleSystem(1000, 5, 0.05, glm::vec3(1.0, 0.0, 0.0));
	}
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {

		glm::vec3 playerPos;
		bool playing = false;
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<Transform> transform;
		for (Entity entity : es.entities_with_components(player, transform)) {
			playing = true;
			player = entity.component<PlayerComponent>();
			transform = entity.component<Transform>();
			ComponentHandle<Physics> physics = entity.component<Physics>();

			Transform cam = *transform.get();
			glm::vec3 offset(0, 4, -9);
			offset = glm::toMat3(normalize(cam.orientation))*offset;
			//cam.pos += offset;

			Transform p_cam = player->camera.getTransform();

			//double v = length(physics->velocity)*0.001;
			double f_pos = 0.05;
			double f_orien = 0.0;
			p_cam.pos += 0.98f*(physics->velocity - physics->acceleration*float(dt))*float(dt);
			p_cam.pos = glm::mix(p_cam.pos, cam.pos, float(1.0 - glm::pow(f_pos, dt)));
			p_cam.orientation = glm::mix(p_cam.orientation, cam.orientation, float(1.0 - glm::pow(f_orien, dt)));

			p_cam.orientation =  p_cam.orientation * glm::rotate(glm::quat(), glm::radians(0.f), glm::vec3(1,0,0));

			player->camera.setTransform(p_cam);

			p_cam.pos += offset;
			Camera c = player->camera;
			c.setTransform(p_cam);

			Renderer::getRenderer().setCamera(c);


			playerPos = transform->pos;// +glm::toMat3(p_cam.orientation)*glm::vec3(0, 0, 4000);
			S->update(dt, transform->pos + glm::toMat3(transform->orientation) * glm::vec3(0.0, 0.0, -3), glm::toMat3(transform->orientation) * glm::vec3(0.0, 0.0, -1.0));
		}




		ComponentHandle<ModelComponent> model;
		for (Entity entity : es.entities_with_components(model, transform)) {
			model = entity.component<ModelComponent>();
			transform = entity.component<Transform>();
			Renderer::getRenderer().addToList(model->mptr, *transform.get());

			player = entity.component<PlayerComponent>();
			ComponentHandle<Projectile> projectile = entity.component<Projectile>();
			if (player) {
				radar.setPlayer(*transform.get());
			}
			else if (!projectile) {
				radar.addPlane(*transform.get());
			}
		}

		ComponentHandle<Terrain> terrain;
		for (Entity entity : es.entities_with_components(terrain)) {
			terrain = entity.component<Terrain>();
			Renderer::getRenderer().setHeightmap(terrain->hmptr);
			Renderer::getRenderer().addToList(terrain->hmptr->buildPatches(playerPos));
		}
		

		ComponentHandle<Equipment> equip;
		for (Entity entity : es.entities_with_components(equip, transform)) {
			equip = entity.component<Equipment>();
			transform = entity.component<Transform>();
			for (int i = 0; i < equip->primary.size(); i++) {
				Transform newTrans;
				newTrans.orientation = transform->orientation;
				newTrans.pos = transform->pos + glm::toMat3(transform->orientation) * equip->primary[i].offset;// *glm::toMat4(equip->primary[i].rot) * glm::scale(equip->primary[i].scale);
				newTrans.orientation = transform->orientation * equip->primary[i].rot;
				newTrans.scale = equip->primary[i].scale;

				Renderer::getRenderer().addToList(equip->primary[i].model, newTrans);
			}
			for (int i = 0; i < equip->special.size(); i++) {
				Transform newTrans;
				newTrans.orientation = transform->orientation;
				newTrans.pos = transform->pos + glm::toMat3(transform->orientation) * equip->special[i].offset;
				newTrans.orientation = transform->orientation * equip->special[i].rot;
				newTrans.scale = equip->special[i].scale;

				Renderer::getRenderer().addToList(equip->special[i].model, newTrans);
			}
		}

		Renderer::getRenderer().RenderScene();
		radar.draw();
		S->render();
		if(playing)
			radar.draw();
	}

	Radar radar;
};