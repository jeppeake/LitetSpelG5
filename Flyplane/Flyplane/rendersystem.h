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
#include "particles.h"
#include "radar.h"
#include "aicomponent.h"
#include "particlecomponent.h"
#include "cameraoncomponent.h"
#include "healthcomponent.h"

using namespace entityx;


struct RenderSystem : public System<RenderSystem> {

	Camera cullingCamera;
	Radar radar;

	RenderSystem()
	{}
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
		bool playing = false;
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<Transform> transform;
		
		glm::vec3 playerPos;
		glm::vec3 playerDir;
		glm::vec3 playerUp;
		glm::quat playerOrientation;
		float hp;
		for (Entity entity : es.entities_with_components(player, transform)) {
			radar.setPlayer(*transform.get());

			playing = true;
			player = entity.component<PlayerComponent>();
			transform = entity.component<Transform>();
			playerPos = transform->pos;
			playerOrientation = transform.get()->orientation;
			playerDir = playerOrientation * glm::vec3(0, 0, 1);
			playerUp = playerOrientation * glm::vec3(0, 1, 0);
			ComponentHandle<Physics> physics = entity.component<Physics>();
			ComponentHandle<HealthComponent> hpComponent = entity.component<HealthComponent>();
			hp = hpComponent->health / hpComponent->maxHP;



			Camera cam;

			glm::vec3 v;
			if (length(physics->velocity) > 0.0001f) {
				v = normalize(physics->velocity);
			}

			glm::mat4 lookAt = glm::lookAt(playerPos, playerPos - playerDir, glm::vec3(0, 1, 0));

			Transform camTrans;
			camTrans.orientation = glm::inverse(glm::quat_cast(lookAt));
			camTrans.pos = playerPos;

			glm::vec3 offset(0, 1, -5.5);

			offset = playerOrientation * offset;
			camTrans.pos += offset;

			cam.setTransform(camTrans);

			cullingCamera = cam;
			Renderer::getRenderer().setCamera(cam);
		}

		ComponentHandle<ModelComponent> model;
		for (Entity entity : es.entities_with_components(model, transform)) {
			model = entity.component<ModelComponent>();
			transform = entity.component<Transform>();
			Renderer::getRenderer().addToList(model->mptr, *transform.get());

			/*player = entity.component<PlayerComponent>();
			ComponentHandle<Projectile> projectile = entity.component<Projectile>();
			if (player) {
				radar.setPlayer(*transform.get());
			}
			else if (!projectile) {
				radar.addPlane(*transform.get());
			}*/
		}

		ComponentHandle<Terrain> terrain;
		for (Entity entity : es.entities_with_components(terrain)) {
			terrain = entity.component<Terrain>();
			Renderer::getRenderer().setHeightmap(terrain->hmptr);
			Renderer::getRenderer().addToList(terrain->hmptr->buildPatches(cullingCamera));
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
		//radar.draw(float(dt));
		ComponentHandle<ParticleComponent> particles;
		for (Entity e : es.entities_with_components(particles)) {
			for (auto &p : particles->systems)
			{
				p->render();
			}
		}

		if (playing) {
			ComponentHandle<AIComponent> ai;

			for (Entity entity : es.entities_with_components(ai, transform)) {
				radar.addPlane(*transform.get());

				glm::vec3 enemyPos = entity.component<Transform>()->pos;
				float length = glm::distance(enemyPos, playerPos);
				glm::vec3 color = glm::vec3(1, 0, 0);
				if (entity.has_component<Target>()) {
					if (entity.component<Target>().get()->is_targeted) {
						color = glm::vec3(0, 1, 0);
					}
				}

				length = 5.0 + length / 100.0f;
				Renderer::getRenderer().addMarker(enemyPos, color, length);
			}
			Renderer::getRenderer().RenderClouds();
			glm::vec3 newPos = playerPos + normalize(playerDir) * 3000.0f;
			Renderer::getRenderer().setCrosshairPos(newPos);
			Renderer::getRenderer().orientation = playerOrientation;
			radar.draw((float)dt);
			Renderer::getRenderer().RenderCrosshair();
			Renderer::getRenderer().RenderHPBar(hp);
		}
		//Renderer::getRenderer().RenderScene();
		//radar.draw(float(dt));
	}
};