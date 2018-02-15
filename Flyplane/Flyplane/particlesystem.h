#pragma once

#include <iostream>
#include "particlecomponent.h"
#include <entityx\entityx.h>
#include <entityx\Event.h>

class ParticleSystem : public entityx::System<ParticleSystem>, public entityx::Receiver<ParticleSystem> {

	size_t searchIndex = 0;
	std::vector<Particles> pool;

	ComputeShader resetShader;
	ComputeShader trailShader;

	ShaderProgram program;
public:
	void configure(entityx::EventManager &eventManager) {
		eventManager.subscribe<entityx::ComponentAddedEvent<ParticleComponent>>(*this);
		eventManager.subscribe<entityx::EntityDestroyedEvent>(*this);

		resetShader.create("particlereset.glsl");
		trailShader.create("particletrail.glsl");

		program.create("partVert.glsl", "partGeom.glsl", "partFrag.glsl");

		for (int i = 0; i < 200; i++) {
			pool.emplace_back(5000);
		}
	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override {

		entityx::ComponentHandle<ParticleComponent> particles;
		for (Entity entity : es.entities_with_components(particles)) {
			trailShader.use();

			auto transform = entity.component<Transform>();
			if (transform) {
				trailShader.uniform("spawn", transform->pos);
				trailShader.uniform("direction", transform->orientation * glm::vec3(0, 0, -1));
			}
			trailShader.uniform("life", 10.f);
			trailShader.uniform("dt", float(dt));

			particles->system->update(trailShader);
		}
	}

	void receive(const entityx::ComponentAddedEvent<ParticleComponent>& event) {
		std::cout << "[DEBUG] Adding ParticleComponent\n";

		auto handle = event.component;

		Particles* free = nullptr;
		for (int i = 0; i < pool.size(); i++) {
			if (!pool[i].used) {
				pool[i].used = true;
				free = &pool[i];
				break;
			}
			searchIndex = (searchIndex + 1) % pool.size();
		}
		if (free) {
			handle->system = free;
			handle->system->program = &program;
			resetShader.use();
			handle->system->update(resetShader);
		} else {
			// maybe resize or something
			std::cout << "[WARNING] out of particles in pool\n";
		}

	}

	// cancer
	void receive(const entityx::EntityDestroyedEvent& event) {
		auto entity = event.entity;
		auto handle = entity.component<ParticleComponent>();
		if (handle) {
			handle->system->used = false;
			//handle->system = nullptr;
		}
	}

};
