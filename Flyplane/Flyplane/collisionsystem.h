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

#include <entityx/entityx.h>
#include <map>

class CollisionSystem : public entityx::System<CollisionSystem>
{
private:
	Heightmap *map;
	PlayingState *state;

	std::map <entityx::Entity::Id, entityx::Entity> to_remove;

	void checkOBBvsPoint(entityx::Entity a, entityx::Entity b)
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
					if (a.has_component<PointComponent>())
						state->addPoints(a.component<PointComponent>().get()->points);
					if (b.has_component<PointComponent>())
						state->addPoints(b.component<PointComponent>().get()->points);
					to_remove[a.id()] = a;
					to_remove[b.id()] = b;
					return;
				}
			}
		}
	}

	void checkOBBvsOBB(entityx::Entity a, entityx::Entity b)
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
						if (a.has_component<PointComponent>())
							state->addPoints(a.component<PointComponent>().get()->points);
						if (b.has_component<PointComponent>())
							state->addPoints(b.component<PointComponent>().get()->points);
						to_remove[a.id()] = a;
						to_remove[b.id()] = b;
						return;
					}
				}
			}
		}
	}

	void checkCollision(entityx::Entity a, entityx::Entity b) 
	{
		if (a.id() == b.id())
			return;

		auto a_model = a.component<ModelComponent>();
		auto b_model = b.component<ModelComponent>();

		auto a_boxes = *a_model->mptr->getBoundingBoxes();
		auto b_boxes = *b_model->mptr->getBoundingBoxes();

		if (!a_boxes.empty() && !b_boxes.empty())
		{
			checkOBBvsOBB(a, b);
		}
		else if (!a_boxes.empty() && b_boxes.empty())
		{
			checkOBBvsPoint(a, b);
		} 
		else if (a_boxes.empty() && !b_boxes.empty())
		{
			checkOBBvsPoint(b, a);
		}
	}
public:
	CollisionSystem(Heightmap *map) : map(map)  {};
	CollisionSystem(Heightmap *map, PlayingState *state) : map(map), state(state) {};
	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		entityx::ComponentHandle<Terrain> terr;
		entityx::Entity terrain;
		for (entityx::Entity entity : es.entities_with_components(terr))
		{
			map = terr->hmptr;
		}
		entityx::ComponentHandle<CollisionComponent> collision;
		entityx::ComponentHandle<Transform> transform;
		entityx::ComponentHandle<ModelComponent> model;
		for(entityx::Entity entity : es.entities_with_components(collision, transform, model))
		{
			// Collision with other entitites 
			for (entityx::Entity other : es.entities_with_components<CollisionComponent, Transform, ModelComponent>())
			{
				if(!entity.has_component<PlayerComponent>() && !other.has_component<PlayerComponent>())
					checkCollision(entity, other);
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
			e.second.destroy();
		}
		to_remove = {};
	}
};