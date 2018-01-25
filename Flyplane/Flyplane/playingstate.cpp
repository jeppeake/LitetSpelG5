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
	entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

	entityx::Entity wep1 = ex.entities.create();

	Model* weaponmodel;

	WeaponStats* stats = new WeaponStats(100, 100, 100, 0.2);

	wep1.assign<Weapon>(stats, weaponmodel);
}

void PlayingState::update(double dt)
{
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
