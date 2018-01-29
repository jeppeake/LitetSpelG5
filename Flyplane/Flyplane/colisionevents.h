#pragma once
#include <entityx/entityx.h>
struct CollisionEvent
{
	CollisionEvent(entityx::Entity ent1, entityx::Entity ent2) : ent1(ent1), ent2(ent2) {}
	entityx::Entity ent1, ent2;
};