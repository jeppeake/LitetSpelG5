#pragma once
#include "heightmap.h"
#include "playercomponent.h"
#include "flightcomponent.h"
#include "modelcomponent.h"
#include "collisionevents.h"
#include "terraincomponent.h"
#include "transform.h"
#include <entityx/entityx.h>
class CollisionSystem : public entityx::System<CollisionSystem>
{
private:
	Heightmap *map;
public:
	CollisionSystem(Heightmap *map) : map(map) {};
	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		entityx::ComponentHandle<Terrain> terr;
		entityx::Entity terrain;
		for (entityx::Entity entity : es.entities_with_components(terr))
		{
			terrain = entity;
			map = terr->hmptr;
		}
		entityx::ComponentHandle<FlightComponent> flight;
		entityx::ComponentHandle<Transform> transform;
		entityx::ComponentHandle<ModelComponent> model;
		for(entityx::Entity entity : es.entities_with_components(flight, transform, model))
		{
			auto boxes = model->mptr->getBoundingBoxes();
			glm::vec3 pos = transform.get()->pos;
			double height = map->heightAt(pos);
			if (boxes->empty())
			{
				if (pos.y <= height)
				{
					events.emit<CollisionEvent>(entity, terrain);
					entity.destroy();
				}
			}
			else
			{
				if (pos.y - height < model->mptr->getBoundingRadius())
				{
					events.emit<CollisionEvent>(entity, terrain);
					entity.destroy();
				}
			}
		}
	}
};