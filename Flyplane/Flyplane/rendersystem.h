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
#include "aicomponent.h"

using namespace entityx;


struct RenderSystem : public System<RenderSystem> {

	Camera cullingCamera;
	Radar radar;

	ParticleSystem *S;
	RenderSystem()
	{
		S = new ParticleSystem(1000, 7, 0.1, glm::vec3(0.7));
	}
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
		bool playing = false;
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<Transform> transform;
		glm::vec3 playerPos;
		glm::vec3 playerDir;
		glm::vec3 playerUp;
		glm::quat playerOrientation;
		for (Entity entity : es.entities_with_components(player, transform)) {
			radar.setPlayer(*transform.get());

			playing = true;
			player = entity.component<PlayerComponent>();
			transform = entity.component<Transform>();
			playerPos = transform->pos;
			playerOrientation = transform.get()->orientation;
			playerDir = glm::mat3(playerOrientation) * glm::vec3(0, 0, 1);
			playerUp = glm::mat3(playerOrientation) * glm::vec3(0, 1, 0);
			ComponentHandle<Physics> physics = entity.component<Physics>();

			Transform cam = *transform.get();
			glm::vec3 offset(0, 4*0.5, -9*0.5);
			offset = glm::toMat3(normalize(cam.orientation))*offset;
			//cam.pos += offset;

			Transform p_cam = player->camera.getTransform();

			//double v = length(physics->velocity)*0.001;
			double f_pos = 0.05;
			double f_orien = 0.0;
			p_cam.pos += 0.99f*(physics->velocity - physics->acceleration*float(dt))*float(dt);
			p_cam.pos = glm::mix(p_cam.pos, cam.pos, float(1.0 - glm::pow(f_pos, dt)));
			p_cam.orientation = glm::mix(p_cam.orientation, cam.orientation, float(1.0 - glm::pow(f_orien, dt)));

			p_cam.orientation =  p_cam.orientation * glm::rotate(glm::quat(), glm::radians(0.f), glm::vec3(1,0,0));

			player->camera.setTransform(p_cam);

			p_cam.pos += offset;
			Camera c = player->camera;
			c.setTransform(p_cam);

			cullingCamera = c;
			if (Input::isKeyDown(GLFW_KEY_Q)) {
				c.setTransform(Transform(glm::vec3(24000, 20000, 24000), glm::quat(0, 0, -0.707, 0.707)));
			}

			
			Renderer::getRenderer().setCamera(c);
			


			glm::vec3 particleDir;
			if (physics)
				if (length(physics->velocity) > 0.0001)
					particleDir = glm::normalize(physics->velocity);
			S->update(dt, transform->pos + glm::toMat3(transform->orientation) * glm::vec3(0.0, 0.0, -2.5) - 3.f*particleDir, -particleDir);
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
		Renderer::getRenderer().RenderScene();
		//radar.draw(float(dt));
		S->render();
		if (playing) {
			glm::vec3 newPos = playerPos + normalize(playerDir) * 3000.0f;
			Renderer::getRenderer().setCrosshairPos(newPos);
			Renderer::getRenderer().orientation = playerOrientation;
			radar.draw((float)dt);
			Renderer::getRenderer().RenderCrosshair();
		}
			
	}
};