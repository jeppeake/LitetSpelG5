#pragma once

#include <iostream>
#include "particlecomponent.h"
#include "addParticleEvent.h"
#include "removeParticleEvent.h"
#include <entityx\entityx.h>
#include <entityx\Event.h>

class ParticleSystem : public entityx::System<ParticleSystem>, public entityx::Receiver<ParticleSystem> {

	size_t searchIndex = 0;
	std::vector<Particles*> pool;

	ComputeShader resetShader;
	ComputeShader trailShader;
	ComputeShader explosionShader;
	ComputeShader engineTrailShader;
	ComputeShader deadTrailShader;
	ComputeShader sparkShader;
	ComputeShader speedShader;
	ComputeShader wingTrailShader;
public:

	~ParticleSystem() {
		std::cout << "[DEBUG] ParticleSystem destructor called\n";
		for (int i = 0; i < pool.size(); i++) {
			delete pool[i];
		}
	}

	void configure(entityx::EventManager &eventManager) {
		eventManager.subscribe<entityx::ComponentAddedEvent<ParticleComponent>>(*this);
		eventManager.subscribe<entityx::EntityDestroyedEvent>(*this);
		eventManager.subscribe<AddParticleEvent>(*this);
		eventManager.subscribe<RemoveParticleEvent>(*this);
		resetShader.create("particlereset.glsl");
		trailShader.create("particletrail.glsl");
		explosionShader.create("explosionCompute.glsl");
		engineTrailShader.create("engineTrail.glsl");
		deadTrailShader.create("deadTrail.glsl");
		sparkShader.create("sparks.glsl");
		speedShader.create("speedPart.glsl");
		wingTrailShader.create("wingTrail.glsl");
		for (int i = 0; i < 40; i++) {
			pool.push_back(new Particles(5000));
		}
	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override {
		entityx::ComponentHandle<ParticleComponent> particles;
		for (Entity entity : es.entities_with_components(particles)) {
			for (size_t i = 0; i < particles->systems.size(); i++)
			{
				if (!particles->systems[i]->isAlive())
				{
					particles->systems[i]->used = false;
					particles->systems[i]->t.restart();
					particles->systems.erase(particles->systems.begin() + i);
					i--;
					std::cout << "[DEBUG] - Removed timed particleSystem \n";
				}
			}

			for (auto &p : particles->systems)
			{
				if (p->used) {
					auto ptr = p;
					chooseUniforms(ptr, entity, dt);
					ptr->update();
				}
			}
		}

		if (Input::isKeyPressed(GLFW_KEY_F7)) {
			resetShader.reload();
			trailShader.reload();
			explosionShader.reload();
			engineTrailShader.reload();
			deadTrailShader.reload();
			sparkShader.reload();
			speedShader.reload();
		}

	}

	void chooseUniforms(Particles *& p, entityx::Entity &entity, const entityx::TimeDelta &dt)
	{
		auto transform = entity.component<Transform>();
		auto physics = entity.component<Physics>();
		auto flight = entity.component<FlightComponent>();
		auto camTrans = Renderer::getRenderer().getCamera().getTransform();
		switch (p->type) {
		case TRAIL:
			//p->setComputeShader(&trailShader);
			//p->setTexture("N/A");
			p->setSize(0.1);
			trailShader.use();
			if (transform) {
				trailShader.uniform("spawn", transform->pos);
				trailShader.uniform("direction", transform->orientation * glm::vec3(0, 0, -1));

				p->params.distFromCam = length(transform->pos - camTrans.pos);
			}
			trailShader.uniform("life", 10.f);
			trailShader.uniform("dt", float(dt));
			break;


		case EXPLOSION:
			//p->setComputeShader(&explosionShader);
			//p->setTexture("explosion");
			p->setSize(p->params.explosion.radius*0.002f);
			explosionShader.use();
			if (transform) {
				explosionShader.uniform("spawn", transform->pos);
				explosionShader.uniform("direction", transform->orientation * glm::vec3(0, 0, -1));

				p->params.distFromCam = length(transform->pos - camTrans.pos);
			}
			explosionShader.uniform("explosionRadius", p->params.explosion.radius);
			explosionShader.uniform("life", p->params.effectLength);
			explosionShader.uniform("dt", float(dt));
			break;


		case SPARKS:
			//p->setComputeShader(&sparkShader);
			//p->setTexture("N/A");
			p->setSize(0.005);
			sparkShader.use();
			sparkShader.uniform("spawn", p->params.sparks.pos);
			if (physics) {
				sparkShader.uniform("velocity", physics->velocity);
			} else {
				sparkShader.uniform("velocity", glm::vec3());
			}
			sparkShader.uniform("life", p->params.effectLength);
			sparkShader.uniform("dt", float(dt));

			p->params.distFromCam = length(p->params.sparks.pos - camTrans.pos);
			break;


		case ENGINE_TRAIL:
			//p->setComputeShader(&engineTrailShader);
			//p->setTexture("engine_fire");
			p->setSize(0.01);
			engineTrailShader.use();
			engineTrailShader.uniform("radius", p->params.engineTrail.radius);
			if (transform) {
				engineTrailShader.uniform("spawn", transform->pos + transform->orientation * p->params.engineTrail.offset);
				engineTrailShader.uniform("direction", transform->orientation * glm::vec3(0, 0, -1));

				p->params.distFromCam = length(transform->pos + transform->orientation * p->params.engineTrail.offset - camTrans.pos);
			}
			if (flight) {
				engineTrailShader.uniform("throttle", flight->throttle - flight->airBrake);
			}
			//engineTrailShader.uniform("life", 0.25f);
			engineTrailShader.uniform("dt", float(dt));
			break;


		case DEAD_TRAIL:
			//p->setComputeShader(&deadTrailShader);
			//p->setTexture("N/A");
			p->setSize(0.1);
			deadTrailShader.use();
			if (transform) {
				deadTrailShader.uniform("spawn", transform->pos);

				p->params.distFromCam = length(transform->pos - camTrans.pos);
			}
			if (physics) {
				deadTrailShader.uniform("direction", -physics->velocity);
			} else {
				deadTrailShader.uniform("direction", glm::vec3());
			}
			deadTrailShader.uniform("life", 10.f);
			deadTrailShader.uniform("dt", float(dt));

			break;


		case SPEED_PARTICLE:
			//p->setComputeShader(&deadTrailShader);
			//p->setTexture("N/A");
			p->setSize(0.05);
			speedShader.use();
			if (transform) {
				speedShader.uniform("spawn", transform->pos);

				p->params.distFromCam = length(transform->pos - camTrans.pos);
			}
			speedShader.uniform("direction", camTrans.orientation * glm::vec3(0, 0, -1));
			speedShader.uniform("life", 6.f);
			speedShader.uniform("dt", float(dt));
			break;


		case WING_TRAIL:
			//p->setComputeShader(&deadTrailShader);
			//p->setTexture("N/A");
			p->setSize(0.01f);
			wingTrailShader.use();
			if (transform) {
				wingTrailShader.uniform("spawn", transform->orientation*p->params.wingTrail.offset + transform->pos);
				wingTrailShader.uniform("up", transform->orientation * glm::vec3(0,1,0));

				p->params.distFromCam = length(transform->orientation*p->params.wingTrail.offset + transform->pos - camTrans.pos);
			}
			if (physics) {
				wingTrailShader.uniform("velocity", physics->velocity);
			}
			wingTrailShader.uniform("life", 6.f);
			wingTrailShader.uniform("dt", float(dt));

			p->params.wingTrail.respawnCounter = (p->params.wingTrail.respawnCounter + 1) % p->numParticles;
			wingTrailShader.uniform("respawnID1", p->params.wingTrail.respawnCounter);

			p->params.wingTrail.respawnCounter = (p->params.wingTrail.respawnCounter + 1) % p->numParticles;
			wingTrailShader.uniform("respawnID2", p->params.wingTrail.respawnCounter);

			wingTrailShader.uniform("respawnID3", (p->params.wingTrail.respawnCounter + 1) % (int)p->numParticles);

			break;

		default:
			// plz no
			break;
		}
	}

	void receive(const entityx::ComponentAddedEvent<ParticleComponent>& event) {
		std::cout << "[DEBUG] Adding ParticleComponent\n";
	}

	// cancer
	
	void receive(const AddParticleEvent& event) {
		auto handle = event.component;

		Particles* free = nullptr;
		for (int i = 0; i < pool.size(); i++) {
			if (!pool[i]->used) {
				pool[i]->used = true;
				free = pool[i];
				break;
			}
			searchIndex = (searchIndex + 1) % pool.size();
		}
		if (free) {
			//free->program = &program;
			free->type = event.type;
			free->params = event.params;
			free->t.restart();
			free->setTimer(event.params.effectLength);
			resetShader.use();
			//free->setComputeShader(&resetShader);
			free->update();
			handle->systems.push_back(free);
		}
		else {
			std::cout << "[WARNING] out of particles in pool, creating new!\n";
			for (int i = 0; i < 20; i++) {
				pool.push_back(new Particles(5000));
			}
			receive(event);
			/*
			for (int i = 0; i < pool.size(); i++) {
				if (!pool[i]->used) {
					pool[i]->used = true;
					free = pool[i];
					break;
				}
				searchIndex = (searchIndex + 1) % pool.size();
			}
			if (free) {
				//free->program = &program;
				free->type = event.type;
				free->t.restart();
				free->setTimer(event.effectLength);
				resetShader.use();
				//free->setComputeShader(&resetShader);
				free->update();
				handle->systems.push_back(free);
			}*/
		}
	}
	void receive(const RemoveParticleEvent& event)
	{
		auto handle = event.component;
		for (unsigned i = 0; i < handle->systems.size(); i++)
		{
			if (handle->systems[i]->type == event.type)
			{
				handle->systems[i]->used = false;
				handle->systems[i]->t.restart();
				handle->systems.erase(handle->systems.begin() + i);
				i--;
				break;
			}
		}
	}
	void receive(const entityx::EntityDestroyedEvent& event) {
		auto entity = event.entity;
		auto handle = entity.component<ParticleComponent>();
		if (handle) {
			for (size_t i = 0; i < handle->systems.size(); i++) {
				handle->systems[i]->used = false;
			}
			//handle->systems.erase(handle->systems.begin(), handle->systems.begin() + handle->systems.size() - 1);
			//handle->system = nullptr;
		}
	}

};
