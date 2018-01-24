#include "playingstate.h"
#include <glm\gtc\constants.hpp>

#include "testsystem.h"
#include "transform.h"
#include "weaponsystem.h"
#include "heightmap.h"
void PlayingState::init()
{
	
	/*
	* add systems
	* 
	* example

	ex.systems.add<System class here>();
	*/
	ex.systems.add<TestSystem>();
	ex.systems.add<WeaponSystem>();

	ex.systems.configure();

	/*
	* add entities
	*
	* example

	auto entity = ex.entities.create();
	entity.assign<Component class here>(Component constructor params);
	*/
	auto entity = ex.entities.create();
	entity.assign<Transform>(glm::vec3(), glm::quat());

	entityx::Entity testWep = ex.entities.create();
	testWep.assign<WeaponStats>(10, 100, 100.0f, 0.01f);
	entityx::Entity testWept = ex.entities.create();
	testWept.assign<WeaponStats>(10, 100, 1000.0f, 0.01f);

	std::vector<Entity> testents;
	testents.push_back(testWep);
	testents.push_back(testWept);
	entityx::Entity equip = ex.entities.create();
	equip.assign<Equipment>(testents);
}

void PlayingState::update()
{
	double dt = deltatime.restart();
	double time = t.elapsed();

	double r = (1 + sin(time))*0.5;
	double g = (1 + sin(time + glm::two_pi<double>() / 3.0))*0.5;
	double b = (1 + sin(time + 2.0*glm::two_pi<double>() / 3.0))*0.5;
	glClearColor(r,g,b, 1.0);

	/*
	ex.systems.update<System class here>(dt);
	*/
	

	ex.systems.update<TestSystem>(dt);
	ex.systems.update<WeaponSystem>(dt);
}
