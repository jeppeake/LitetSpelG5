#pragma once
#include "heightmap.h"
#include "playercomponent.h"
#include "flightcomponent.h"
#include "modelcomponent.h"
#include "collisionevents.h"
#include "terraincomponent.h"
#include "transform.h"
#include "collisioncomponent.h"
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
			map = terr->hmptr;
		}
		entityx::ComponentHandle<CollisionComponent> collision;
		entityx::ComponentHandle<Transform> transform;
		entityx::ComponentHandle<ModelComponent> model;
		for(entityx::Entity entity : es.entities_with_components(collision, transform, model))
		{
			auto boxes = *model->mptr->getBoundingBoxes();

			entityx::ComponentHandle<CollisionComponent> ocollision;
			entityx::ComponentHandle<Transform> otransform;
			entityx::ComponentHandle<ModelComponent> omodel;
			for (entityx::Entity other : es.entities_with_components(ocollision, otransform, omodel))
			{
				auto oboxes = omodel->mptr->getBoundingBoxes();
				if (entity.id() == other.id())
					continue;

				float distance = length(transform->pos - otransform->pos);
				float radii = model->mptr->getBoundingRadius() + omodel->mptr->getBoundingRadius();
				if (distance < radii) 
				{
					//std::cout << "Checking OBB-OBB collision!!!\n";
					for (int i = 0; i < boxes.size(); i++) {
						boxes[i].setTransform(*transform.get());
						//auto c = boxes[i].getWorldCenter();
						//std::cout << c.x << ", " << c.y << ", " << c.z << "\n";
					}
					for (int i = 0; i < oboxes->size(); i++) {
						(*oboxes)[i].setTransform(*otransform.get());
					}

					for (int i = 0; i < boxes.size(); i++) {
						for (int j = 0; j < oboxes->size(); j++) {
							if ((*oboxes)[j].intersect(boxes[i])) {
								if(entity.has_component<FlightComponent>())
									entity.remove<FlightComponent>();
								//other.remove<FlightComponent>();
							}
						}
					}
				}
			}

			glm::vec3 pos = transform.get()->pos;
			double height = map->heightAt(pos);
			if (boxes.empty())
			{
				if (pos.y <= height)
				{
					events.emit<CollisionEvent>(entity, terrain);
					entity.destroy();
				}
			}
			else
			{
				if (pos.y - height < model->mptr->getBoundingRadius() + 20)
				{
					std::cout << "Checking collision with terrain!!!\n";
					for (size_t i = 0; i < boxes.size(); i++)
					{
						boxes[i].setTransform(*transform.get());
						if (boxes[i].intersect(terr->hmptr))
						{
							events.emit<CollisionEvent>(entity, terrain);
							entity.destroy();
							break;
						}
					}
				}
			}
		}
	}
};