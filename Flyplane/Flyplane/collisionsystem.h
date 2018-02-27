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
#include <map>

class CollisionSystem : public entityx::System<CollisionSystem>
{
private:
	Heightmap *map;
	PlayingState *state;
	sf::Sound hitSound;

	std::map<entityx::Entity::Id, entityx::Entity> to_remove;

	void checkOBBvsPoint(entityx::Entity a, entityx::Entity b, entityx::EventManager &es)
	{
		auto a_trans = a.component<Transform>();
		auto a_model = a.component<ModelComponent>();

		auto b_trans = b.component<Transform>();
		auto b_model = b.component<ModelComponent>();

		auto a_boxes = *a_model->mptr->getBoundingBoxes();

		for (int i = 0; i < a_boxes.size(); i++)
		{
			a_boxes[i].setTransform(*a_trans.get());
		}

		float distance = length(a_trans->pos - b_trans->pos);
		float radii = a_model->mptr->getBoundingRadius();
		if (distance < radii)
		{
			for (int i = 0; i < a_boxes.size(); i++)
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
		auto a_trans = a.component<Transform>();
		auto a_model = a.component<ModelComponent>();

		auto b_trans = b.component<Transform>();
		auto b_model = b.component<ModelComponent>();

		auto a_boxes = *a_model->mptr->getBoundingBoxes();
		auto b_boxes = *b_model->mptr->getBoundingBoxes();

		for (int i = 0; i < a_boxes.size(); i++)
		{
			a_boxes[i].setTransform(*a_trans.get());
		}
		for (int i = 0; i < b_boxes.size(); i++)
		{
			b_boxes[i].setTransform(*b_trans.get());
		}
		

		float distance = length(a_trans->pos - b_trans->pos);
		float radii = a_model->mptr->getBoundingRadius() + b_model->mptr->getBoundingRadius();
		if (distance < radii)
		{
			for (int i = 0; i < a_boxes.size(); i++)
			{
				for (int j = 0; j < b_boxes.size(); j++)
				{
					if (a_boxes[i].intersect(b_boxes[j]))
					{
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
					hitSound.play();
				}
				auto handle = a.component<ParticleComponent>();
				if (!handle)
				{
					handle = a.assign<ParticleComponent>();
				}
				es.emit<AddParticleEvent>(SPARKS, handle, 3);
			}
		}
	}

	void handleDrop(entityx::Entity player, entityx::Entity drop) {
		std::cout << "Picked up drop! ";

		switch (drop.component<DropComponent>()->type) {
		case DropComponent::Health:
			player.component<HealthComponent>()->health += drop.component<DropComponent>()->amount;
			if (player.component<HealthComponent>()->health > player.component<HealthComponent>()->maxHP) {
				player.component<HealthComponent>()->health = player.component<HealthComponent>()->maxHP;
			}
			std::cout << " Health!" << std::endl;
			break;
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

		auto a_model = a.component<ModelComponent>();
		auto b_model = b.component<ModelComponent>();

		auto a_boxes = *a_model->mptr->getBoundingBoxes();
		auto b_boxes = *b_model->mptr->getBoundingBoxes();

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
public:
	CollisionSystem(Heightmap *map) : map(map) {
		hitSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("tink"));
		hitSound.setRelativeToListener(true);
		hitSound.setPosition(0, 0, 0);
	};
	CollisionSystem(Heightmap *map, PlayingState *state) : map(map), state(state) {
		hitSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("tink"));
		hitSound.setRelativeToListener(true);
		hitSound.setPosition(0, 0, 0);
	};
	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		/******************
		**     DEBUG     **
		******************/



		/******************
		**    \DEBUG     **
		******************/


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
		entityx::ComponentHandle<ModelComponent> model;
		for(entityx::Entity entity : es.entities_with_components(collision, transform, model))
		{

			// Collision with other entitites 
			if (entity.has_component<Projectile>()) {
				if (entity.has_component<FactionPlayer>()) {
					for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, ModelComponent, FactionEnemy>()) {
						checkCollision(entity, other, events);
					}
				} else if (entity.has_component<FactionEnemy>()) {
					for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, ModelComponent, FactionPlayer>()) {
						checkCollision(entity, other, events);
					}
				} else {
					// ???
				}
			}
			else {
				if (entity.has_component<FlightComponent>()) {
					for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, ModelComponent, FlightComponent>()) {
						checkCollision(entity, other,events);
					}
				}
				else if (entity.has_component<DropComponent>()) {
						for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, ModelComponent, PlayerComponent>()) {
							checkCollision(entity, other, events);
						}
				}
			}
			
			if (!map && entity.has_component<HouseComponent>()) {
				continue;
			}
			auto boxes = *model->mptr->getBoundingBoxes();

			// Collision with terrain
			glm::vec3 pos = transform.get()->pos;
			double height = map->heightAt(pos);
			if (boxes.empty())
			{
				// Terrain point collision
				if (pos.y <= height)
				{
					events.emit<CollisionEvent>(entity, terrain);
					to_remove[entity.id()] = entity;
				}
			}
			else
			{
				// Terrain OBB collision
				if (pos.y - height < model->mptr->getBoundingRadius())
				{
					for (size_t i = 0; i < boxes.size(); i++)
					{
						boxes[i].setTransform(*transform.get());
						if (boxes[i].intersect(terr->hmptr))
						{
							events.emit<CollisionEvent>(entity, terrain);
							to_remove[entity.id()] = entity;
							break;
						}
					}
				}
			}
		}


		for (auto& e : to_remove)
		{
			if (e.second.has_component<AIComponent>()) {
				std::cout << "COLLISION DEATH\n";
			}
			else if (e.second.has_component<DropComponent>()) {
				std::cout << "Removed drop\n";
			}
			e.second.destroy();
		}
		to_remove = {};
	}
};