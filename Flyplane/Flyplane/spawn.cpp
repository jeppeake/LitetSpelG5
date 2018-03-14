#include "spawn.h"

#include "assetloader.h"
#include "housecomponent.h"
#include "collisioncomponent.h"
#include "modelcomponent.h"

void Spawn::landmarks(entityx::EntityManager & es) {
	{
		entityx::Entity tower = es.create();
		tower.assign<HouseComponent>();
		tower.assign<CollisionComponent>();
		tower.assign<ModelComponent>(AssetLoader::getLoader().getModel("monolith"));
		glm::vec3 pos(3000, 0, -7000);
		pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(pos);
		float size = 200.f;
		tower.assign<Transform>(pos, glm::quat(glm::vec3(0, glm::pi<float>() / 4.f, 0)), glm::vec3(size));
	}

	{
		entityx::Entity pyramid = es.create();
		pyramid.assign<HouseComponent>();
		pyramid.assign<CollisionComponent>();
		pyramid.assign<ModelComponent>(AssetLoader::getLoader().getModel("pyramid"));
		glm::vec3 pos(3891, 0, 5300);
		pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(pos);
		float size = 400.f;
		pyramid.assign<Transform>(pos, glm::quat(glm::vec3(0, glm::pi<float>() / 16.f, 0)), glm::vec3(size));
	}
	{
		entityx::Entity pyramid = es.create();
		pyramid.assign<HouseComponent>();
		pyramid.assign<CollisionComponent>();
		pyramid.assign<ModelComponent>(AssetLoader::getLoader().getModel("pyramid"));
		glm::vec3 pos(4300, 0, 5700);
		pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(pos);
		float size = 300.f;
		pyramid.assign<Transform>(pos, glm::quat(glm::vec3(0, glm::pi<float>() / 16.f, 0)), glm::vec3(size));
	}



}
