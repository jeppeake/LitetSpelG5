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


	{
		entityx::Entity mindcontrol = es.create();
		mindcontrol.assign<HouseComponent>();
		mindcontrol.assign<CollisionComponent>();
		mindcontrol.assign<ModelComponent>(AssetLoader::getLoader().getModel("mindcontrol"));
		glm::vec3 pos(-8000, 0, -4900);
		pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(pos);
		float size = 150.f;
		mindcontrol.assign<Transform>(pos, glm::quat(glm::vec3(0, glm::pi<float>() / 16.f, 0)), glm::vec3(size));
	}


	{
		entityx::Entity radome = es.create();
		radome.assign<HouseComponent>();
		radome.assign<CollisionComponent>();
		radome.assign<ModelComponent>(AssetLoader::getLoader().getModel("radome"));
		glm::vec3 pos(-2730, 0, 4750);
		pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(pos);
		float size = 20.f;
		radome.assign<Transform>(pos, glm::quat(glm::vec3(0, glm::pi<float>() / 16.f, 0)), glm::vec3(size));
	}
	{
		entityx::Entity top = es.create();
		top.assign<HouseComponent>();
		top.assign<CollisionComponent>();
		top.assign<ModelComponent>(AssetLoader::getLoader().getModel("radometop"));
		top.assign<RotateComponent>();
		glm::vec3 pos(-2730, 0, 4750);
		float size = 20.f;
		pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(pos);
		top.assign<Transform>(pos, glm::quat(glm::vec3(0, glm::pi<float>() / 16.f, 0)), glm::vec3(size));
	}


	{
		entityx::Entity fort = es.create();
		fort.assign<HouseComponent>();
		fort.assign<CollisionComponent>();
		fort.assign<ModelComponent>(AssetLoader::getLoader().getModel("fort"));
		glm::vec3 pos(-7722, 0, 1200);
		pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(pos) + 10;
		float size = 450.f;
		fort.assign<Transform>(pos, glm::quat(glm::vec3(0, glm::pi<float>() / 16.f, 0)), glm::vec3(size));
	}


}
