#include "playingstate.h"
#include <glm\gtc\constants.hpp>

#include "physicssystem.h"
#include "transform.h"
#include "weaponsystem.h"
#include "rendersystem.h"
#include "heightmap.h"
#include "playercomponent.h"
#include "playersystem.h"
Model m;
Model weaponmodel;
void PlayingState::init()
{
	m.load("assets/MIG-212A.fbx");
	/*
	* add systems
	* 
	* example

	ex.systems.add<System class here>();
	*/
	ex.systems.add<PhysicsSystem>();
	ex.systems.add<WeaponSystem>();
	ex.systems.add<RenderSystem>();
	ex.systems.add<PlayerSystem>();
	ex.systems.configure();

	/*
	* add entities
	*
	* example

	auto entity = ex.entities.create();
	entity.assign<Component class here>(Component constructor params);
	*/
	
	auto v = []() {
		return (rand() % 1000 - 500)*0.05;
	};


	for (int i = 0; i < 200; i++) {
		auto entity = ex.entities.create();
		glm::vec3 pos(rand() % 100, rand() % 100, rand() % 100);
		glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		entity.assign<Transform>(pos, normalize(orien));
		entity.assign<Physics>(1000.0, 1.0, 20.f, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
		entity.assign <ModelComponent>(&m);
	}

	auto entity = ex.entities.create();
	glm::vec3 pos(rand() % 100, rand() % 100, rand() % 100);
	glm::quat orien(1,0,0,0);
	entity.assign<Transform>(pos, normalize(orien));
	entity.assign<Physics>(1000.0, 1.0, 2.f, glm::vec3(v(), v(), v()), glm::vec3(0.0, 0.0, 0.0));
	entity.assign <ModelComponent>(&m);
	entity.assign <PlayerComponent>();
	


	std::vector<Weapon> weapons;

	
	weaponmodel.load("assets/testbullet.fbx");

	WeaponStats* stats = new WeaponStats(100, 100, 100, 0.2, 0.5f, true);
	WeaponStats* stats2 = new WeaponStats(100, 100, 1000, 0.2, 0.02f, true);
	WeaponStats* bomb = new WeaponStats(10, 100, 0, 0.2, 0.5f, true);

	weapons.emplace_back(stats, &weaponmodel, true, glm::vec3(-1,-0.2,0));
	weapons.emplace_back(stats2, &weaponmodel, true, glm::vec3(-2, -0.2, 0));
	weapons.emplace_back(stats, &weaponmodel, true, glm::vec3(1, -0.2, 0));
	weapons.emplace_back(bomb, &weaponmodel, true, glm::vec3(0, -0.2, -0.5));

	entity.assign <Equipment>(weapons);
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
	
	if(Input::isKeyDown(GLFW_KEY_SPACE))
		std::cout << ex.entities.size() << "\n";



	ex.systems.update<PlayerSystem>(dt);
	ex.systems.update<PhysicsSystem>(dt);
	ex.systems.update<WeaponSystem>(dt);

	ex.systems.update<RenderSystem>(dt);
}
