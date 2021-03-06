#pragma once
#include "healthcomponent.h"
#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include <iostream>
#include "pointcomponent.h"
#include "explosivecomponenet.h"
#include "playingstate.h"

using namespace entityx;

struct HealthSystem : public entityx::System<HealthSystem> {
	sf::Sound explosionSound;

	PlayingState* state;
	HealthSystem(PlayingState *state) : state(state) {
		explosionSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("explosion"));
		explosionSound.setRelativeToListener(true);
		explosionSound.setPosition(0, 0, 0);
	};
	entityx::ComponentHandle<HealthComponent> health;
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		for (Entity entity : es.entities_with_components(health)) {
			if (health->health <= 0 && !health->isDead) {
				health->isDead = true;
				std::cout << "Entity got killed." << "\n";
				if (entity.has_component<PointComponent>())
					state->addPoints(entity.component<PointComponent>()->points);
				if (entity.has_component<FlightComponent>())
				{
					entity.remove<FlightComponent>();
					if (entity.has_component<Target>())
					{
						entity.remove<Target>();
					}
					if (entity.has_component<SoundComponent>()) {
						entity.remove<SoundComponent>();
					}
				}

				if (entity.has_component<PlayerComponent>()) {
					if(!entity.has_component<LifeTimeComponent>())
						entity.assign<LifeTimeComponent>(1.5f);
					explosionSound.play();
				}

				if (entity.has_component<ExplosiveComponenet>()) {
					ComponentHandle<ExplosiveComponenet> explosive = entity.component<ExplosiveComponenet>();
					Entity boom = es.create();
					boom.assign<ExplosionComponent>(explosive->explodeDamage, explosive->explodeRadius);
					boom.assign<Transform>(entity.component<Transform>()->pos + glm::vec3(0,100,0));
					boom.assign<BurstSoundComponent>(*AssetLoader::getLoader().getSoundBuffer("explosion"), entity.component<Transform>()->pos + glm::vec3(0, 100, 0), true, explosive->explodeRadius, 0.01, 1, 100);
					auto handle = boom.assign<ParticleComponent>();
					ParticleParameters params;
					params.effectLength = 3.f;
					params.explosion.radius = explosive->explodeRadius;
					events.emit<AddParticleEvent>(EXPLOSION, handle, params);
				}

				auto handle = entity.component<ParticleComponent>();
				if (!handle)
				{
					handle = entity.assign<ParticleComponent>();
				}
				ParticleParameters p;
				p.effectLength = 3.f;
				p.explosion.radius = 20.f;
				events.emit<AddParticleEvent>(EXPLOSION, handle, p);
				events.emit<RemoveParticleEvent>(TRAIL, handle);
				events.emit<RemoveParticleEvent>(ENGINE_TRAIL, handle);
				events.emit<AddParticleEvent>(DEAD_TRAIL, handle);
			}
		}
	}
};