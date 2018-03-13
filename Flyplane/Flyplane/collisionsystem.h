#pragma once
#include "heightmap.h"
#include "playercomponent.h"
#include "flightcomponent.h"
#include "modelcomponent.h"
#include "collisionevents.h"
#include "terraincomponent.h"
#include "transform.h"
#include "collisioncomponent.h"
#include "pointcomponent.h"
#include "factioncomponents.h"
#include <entityx/entityx.h>
#include "missilecomponent.h"
#include "dropcomponent.h"
#include "playingstate.h"
#include "housecomponent.h"
#include "lifetimecomponent.h"
#include <map>

class CollisionSystem : public entityx::System<CollisionSystem>
{
private:
	Heightmap *map;
	PlayingState *state;
	sf::Sound hitSound;
	sf::Sound ammoDropSound;
	sf::Sound healthDropSound;
	sf::Sound explosionSound;
	std::vector<sf::Sound> playerHitSounds;

	std::map<entityx::Entity::Id, entityx::Entity> to_remove;

	void checkOBBvsPoint(entityx::Entity a, entityx::Entity b, entityx::EventManager &es)
	{
		// OBB
		std::vector<BoundingBox>& a_boxes = a.component<CollisionComponent>()->boxes;

		// Point
		auto b_trans = b.component<Transform>();

		for (int i = 0; i < a_boxes.size(); i++)
		{
			float distance = length(a_boxes[i].worldCenter - b_trans->pos);
			float radii = a_boxes[i].boundingRadius;
			if (distance < radii)
			{
				if (a_boxes[i].intersect(b_trans->pos))
				{
					handleCollision(a, b, es);
					return;
				}
			}
		}
	}

	void checkOBBvsOBB(entityx::Entity a, entityx::Entity b, entityx::EventManager &es)
	{
		std::vector<BoundingBox>& a_boxes = a.component<CollisionComponent>()->boxes;
		std::vector<BoundingBox>& b_boxes = b.component<CollisionComponent>()->boxes;

		for (int i = 0; i < a_boxes.size(); i++) {
			for (int j = 0; j < b_boxes.size(); j++) {

				float distance = length(a_boxes[i].worldCenter - b_boxes[j].worldCenter);
				float radii = a_boxes[i].boundingRadius + b_boxes[j].boundingRadius;

				if (distance < radii) {
					if (a_boxes[i].intersect(b_boxes[j])) {
						handleCollision(a, b, es);
						return;
					}
				}
			}
		}
	}

	void handleHealth(entityx::Entity a, entityx::Entity b, entityx::EventManager &es) {
		if (a.has_component<HealthComponent>()) {
			auto health = a.component<HealthComponent>();

			if (b.has_component<Missile>()) {
				auto missile = b.component<Missile>().get();
				missile->shouldExplode = true;
			}
			else if (b.has_component<Projectile>()) {
				auto projectile = b.component<Projectile>().get();
				health->health -= projectile->damage;
				std::cout << "Did " << projectile->damage << " damage." << "\n";
				// remove the projectile
				to_remove[b.id()] = b;

				if (b.has_component<FactionPlayer>()) {
					if(hitSound.getStatus() != sf::Sound::Playing)
						hitSound.play();
				}
				

				auto cameraOn = a.component<CameraOnComponent>();
				if (cameraOn) {
					cameraOn->shake += 1;
				}


				auto btransform = b.component<Transform>();


				if (a.has_component<PlayerComponent>()) {
					for (int i = 0; i < playerHitSounds.size(); i++) {
						if (playerHitSounds[i].getStatus() != sf::Sound::Playing) {
							glm::vec3 pos = btransform->pos;
							playerHitSounds[i].setPosition(pos.x, pos.y, pos.z);
							playerHitSounds[i].play();
							break;
						}
					}
				}

				auto handle = a.component<ParticleComponent>();
				if (!handle)
				{
					handle = a.assign<ParticleComponent>();
				}

				ParticleParameters p;
				p.effectLength = 1.f;
				if(btransform)
					p.sparks.pos = btransform->pos;
				es.emit<AddParticleEvent>(SPARKS, handle, p);
			}
			else if (b.has_component<HouseComponent>()) {
				if (health) {
					health->health = -1;
				}
				if (a.has_component<Physics>())
					a.remove<Physics>();
				if (a.has_component<CollisionComponent>())
					a.remove<CollisionComponent>();

			}
		}
	}

	void handleDrop(entityx::Entity player, entityx::Entity drop) {
		std::cout << "Picked up drop! ";
		switch (drop.component<DropComponent>()->type) {
		case DropComponent::Health:
			healthDropSound.play();
			if (player.has_component<HealthComponent>())
				player.component<HealthComponent>()->health = player.component<HealthComponent>()->maxHP;
			
			std::cout << " Health!" << std::endl;
			break;
		case DropComponent::Ammo:
			ammoDropSound.play();
			if (player.has_component<Equipment>())
				player.component<Equipment>()->refill();
		
		/*
		case DropComponent::Bigboi:
			if (player.has_component<Equipment>())
				player.component<Equipment>()->addSpecialWeapon(*AssetLoader::getLoader().getWeapon("Bigboi"));
			std::cout << " Bigboi!" << std::endl;
			break;
		case DropComponent::Fishrod:
			if (player.has_component<Equipment>())
				player.component<Equipment>()->addSpecialWeapon(*AssetLoader::getLoader().getWeapon("Fishrod"));
			std::cout << " Fishrod!" << std::endl;
			break;
		case DropComponent::HAAM_166:
			if (player.has_component<Equipment>())
				player.component<Equipment>()->addSpecialWeapon(*AssetLoader::getLoader().getWeapon("HAAM-166"));
			std::cout << " HAAM-166!" << std::endl;
			break;
		case DropComponent::Micromissile:
			if (player.has_component<Equipment>())
				player.component<Equipment>()->addSpecialWeapon(*AssetLoader::getLoader().getWeapon("Micromissile"));
			std::cout << " Micromissile!" << std::endl;
			break;
		case DropComponent::Rocketpod:
			if (player.has_component<Equipment>())
				player.component<Equipment>()->addSpecialWeapon(*AssetLoader::getLoader().getWeapon("Rocketpod"));
			std::cout << " Rocketpod!" << std::endl;
			break;
		case DropComponent::Rodfish:
			if (player.has_component<Equipment>())
				player.component<Equipment>()->addSpecialWeapon(*AssetLoader::getLoader().getWeapon("Rodfish"));
			std::cout << " Rodfish!" << std::endl;
			break;
		case DropComponent::SHAAM_200:
			if (player.has_component<Equipment>())
				player.component<Equipment>()->addSpecialWeapon(*AssetLoader::getLoader().getWeapon("SHAAM-200"));
			std::cout << " SHAAM-200!" << std::endl;
			break;
		*/
		}
	}

	
	void handleCollision(entityx::Entity a, entityx::Entity b, entityx::EventManager &es) {
		handleHealth(a, b, es);
		handleHealth(b, a, es);

		/*if (a.has_component<PointComponent>())
			state->addPoints(a.component<PointComponent>().get()->points);
		if (b.has_component<PointComponent>())
			state->addPoints(b.component<PointComponent>().get()->points);*/

		if (a.has_component<FlightComponent>() && b.has_component<FlightComponent>()) {
			to_remove[a.id()] = a;
			to_remove[b.id()] = b;
		}
		else if (a.has_component<DropComponent>()) {
			//std::cout << "Collision with drop" << std::endl;
			handleDrop(b, a);
			to_remove[a.id()] = a;
		}
		else if (b.has_component<DropComponent>()) {
			handleDrop(a, b);
			to_remove[b.id()] = b;
		}
	}

	void checkCollision(entityx::Entity a, entityx::Entity b, entityx::EventManager &es)
	{
		if (a.id() == b.id())
			return;

		std::vector<BoundingBox>& a_boxes = a.component<CollisionComponent>()->boxes;
		std::vector<BoundingBox>& b_boxes = b.component<CollisionComponent>()->boxes;

		if (!a_boxes.empty() && !b_boxes.empty())
		{
			checkOBBvsOBB(a, b, es);
		}
		else if (!a_boxes.empty() && b_boxes.empty())
		{
			checkOBBvsPoint(a, b, es);
		} 
		else if (a_boxes.empty() && !b_boxes.empty())
		{
			checkOBBvsPoint(b, a, es);
		}
	}


	void handleTerrainCollision(entityx::Entity entity, entityx::EventManager &event) {
		if (entity.has_component<Projectile>()) {
			if (entity.has_component<Missile>()) {
				entity.component<Missile>()->shouldExplode = true;
			}
			else {
				to_remove[entity.id()] = entity;
			}
		} else {

			if (!entity.has_component<LifeTimeComponent>()) {
				double life = 20.0;
				if (entity.has_component<PlayerComponent>()) {
					life = 1.0;
				}

				entity.assign<LifeTimeComponent>(life);
			}

			if (entity.has_component<HealthComponent>()) {
				entity.component<HealthComponent>()->health = 0;
			}

			

			auto handle = entity.component<ParticleComponent>();
			if (!handle) {
				handle = entity.assign<ParticleComponent>();
			}
			ParticleParameters p;
			p.effectLength = 3.f;
			p.explosion.radius = 50.f;
			event.emit<AddParticleEvent>(EXPLOSION, handle, p);


			auto transform = entity.component<Transform>();
			if (transform) {
				auto p = transform->pos;
				explosionSound.setPosition(p.x, p.y, p.z);
			}
			explosionSound.play();
			

			entity.remove<CollisionComponent>();
			if(entity.has_component<Physics>())
				entity.remove<Physics>();
			if (entity.has_component<AIComponent>())
				entity.remove<AIComponent>();
		}
	}


	void setupSounds() {
		hitSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("tink"));
		hitSound.setRelativeToListener(true);
		hitSound.setPosition(0, 0, 0);
		hitSound.setVolume(50);

		ammoDropSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("ammodrop"));
		ammoDropSound.setRelativeToListener(true);
		ammoDropSound.setPosition(0, 0, 0);
		ammoDropSound.setVolume(50);

		healthDropSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("healthdrop"));
		healthDropSound.setRelativeToListener(true);
		healthDropSound.setPosition(0, 0, 0);
		healthDropSound.setVolume(50);

		for (int i = 0; i < 10; i++) {
			playerHitSounds.emplace_back();
			playerHitSounds.back().setBuffer(*AssetLoader::getLoader().getSoundBuffer("playerhit"));
		}
		
		//playerHitSound.

		explosionSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("explosion"));
		explosionSound.setPosition(0, 0, 0);
		explosionSound.setMinDistance(400.f);
		explosionSound.setAttenuation(0.5f);
	}
public:
	CollisionSystem(Heightmap *map) : map(map) {
		setupSounds();
	};
	CollisionSystem(Heightmap *map, PlayingState *state) : map(map), state(state) {
		setupSounds();
	};

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		


		Heightmap* map = nullptr;
		entityx::ComponentHandle<Terrain> terr;
		entityx::Entity terrain;
		for (entityx::Entity entity : es.entities_with_components(terr)) {
			map = terr->hmptr;
		}
		if (!map) {
			std::cout << "WARNING: could not find map in collision system\n";
		}

		entityx::ComponentHandle<CollisionComponent> collision;
		entityx::ComponentHandle<Transform> transform;
		entityx::ComponentHandle<ModelComponent> modelComp;
		for (entityx::Entity entity : es.entities_with_components(collision, transform, modelComp)) {
			collision->boxes = *modelComp->mptr->getBoundingBoxes();
			for (auto& box : collision->boxes) {
				box.setTransform(*transform.get());
			}
		}
		
		
		
		for(entityx::Entity entity : es.entities_with_components(collision, transform))
		{
			// Collision with other entitites 
			if (entity.has_component<Projectile>()) {
				if (entity.has_component<FactionPlayer>()) {
					for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, FactionEnemy>()) {
						checkCollision(entity, other, events);
					}
				} else if (entity.has_component<FactionEnemy>()) {
					for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, FactionPlayer>()) {
						checkCollision(entity, other, events);
					}
				} else {
					// ???
				}
			}
			else {
				if (entity.has_component<FlightComponent>()) {
					for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, FlightComponent>()) {
						checkCollision(entity, other,events);
					}
				} else if (entity.has_component<DropComponent>()) {
					for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, PlayerComponent>()) {
						checkCollision(entity, other, events);
					}
				}
			}

			// if entity is house skip check terrain or other houses
			if (entity.has_component<HouseComponent>()) {
				continue;
			}

			// check with houses
			for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, HouseComponent>()) {
				checkCollision(entity, other, events);
			}

			if (!map || !entity.has_component<CollisionComponent>()) {
				continue;
			}

			std::vector<BoundingBox>& boxes = collision->boxes;
			auto model = entity.component<ModelComponent>();

			// Collision with terrain
			glm::vec3 pos = transform.get()->pos;
			double height = map->heightAt(pos);
			if (boxes.empty())
			{
				// Terrain point collision
				if (pos.y <= height)
				{
					handleTerrainCollision(entity, events);
					//to_remove[entity.id()] = entity;
				}
			}
			else
			{
				// Terrain OBB collision
				if (pos.y - height < model->mptr->getBoundingRadius())
				{
					for (size_t i = 0; i < boxes.size(); i++)
					{
						if (boxes[i].intersect(terr->hmptr))
						{
							handleTerrainCollision(entity, events);
							//to_remove[entity.id()] = entity;
							break;
						}
					}
				}
			}
		}


		for (auto& e : to_remove)
		{
			if (e.second.has_component<AIComponent>()) {
				std::cout << "AI COLLISION DEATH\n";
			}
			else if (e.second.has_component<DropComponent>()) {
				std::cout << "Removed drop\n";
			}
			e.second.destroy();
		}
		to_remove = {};
	}
};