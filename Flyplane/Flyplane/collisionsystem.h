#pragma once
#include "heightmap.h"
#include "playercomponent.h"
#include "flightcomponent.h"
#include "modelcomponent.h"
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
		entityx::ComponentHandle<FlightComponent> flight;
		entityx::ComponentHandle<Transform> transform;
		entityx::ComponentHandle<ModelComponent> model;
		for(entityx::Entity entity : es.entities_with_components(flight, transform, model))
		{
			
		}
	}
};