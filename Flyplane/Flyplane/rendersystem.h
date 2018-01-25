#pragma once


#include <entityx\entityx.h>
#include <entityx\Entity.h>

#include "renderer.h"
#include "modelcomponent.h"
#include "transform.h"
#include "playercomponent.h"

using namespace entityx;

struct RenderSystem : public System<RenderSystem> {

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
		ComponentHandle<PlayerComponent> player;
		for (Entity entity : es.entities_with_components(player)) {
			player = entity.component<PlayerComponent>();
			Renderer::getRenderer().setCamera(player->camera);
		}


		ComponentHandle<ModelComponent> model;
		ComponentHandle<Transform> transform;
		for (Entity entity : es.entities_with_components(model, transform)) {
			model = entity.component<ModelComponent>();
			transform = entity.component<Transform>();
			Renderer::getRenderer().Render(*model->mptr, *transform.get());
		}
	}

};