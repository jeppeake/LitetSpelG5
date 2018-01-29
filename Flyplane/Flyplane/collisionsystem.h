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
			glm::vec3 pos = transform.get()->pos;
			double height = map->heightAt(pos);
			if (entity.has_component<PlayerComponent>())
			{
				//std::cout << "Entity: " << pos.y << " : Terrain : " << height << std::endl;
				//std::cout << "Height over terrain: " << pos.y - height << "\n";
			}
			if (pos.y <= height)
			{
				events.emit<CollisionEvent>(entity, terrain);
				entity.destroy();
			}
		}
	}
};