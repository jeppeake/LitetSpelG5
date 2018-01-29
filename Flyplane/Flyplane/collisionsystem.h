#pragma once
#include <entityx/entityx.h>
struct CollisionSystem : public entityx::System<CollisionSystem>
{
	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{

	}
};