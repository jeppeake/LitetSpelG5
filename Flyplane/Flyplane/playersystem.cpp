#include "playersystem.h"

#include "terraincomponent.h"
#include "healthcomponent.h"
#include "targetcomponent.h"
#include "lifetimecomponent.h"
#include "factioncomponents.h"
#include "renderer.h"

void spawnFlare(Entity flare, glm::vec3 dir, Transform* transform, Physics* physics, EventManager & events);

PlayerSystem::PlayerSystem()
{
	warningSound.setRelativeToListener(true);
	warningSound.setPosition(0, 0, 0);
	warningSound.setVolume(25);
	warningSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("warning"));
}
void PlayerSystem::update(EntityManager & es, EventManager & events, TimeDelta dt)
{
	ComponentHandle<Transform> transform;
	ComponentHandle<Physics> physics;
	ComponentHandle<PlayerComponent> player;
	ComponentHandle<FlightComponent> flight;
	ComponentHandle<Target> target;
	for (Entity entity : es.entities_with_components(physics, transform, player, flight, target)) {
		if(entity.component<Target>()->is_targeted) {
			//std::cout << "GO EVASIVE!\n";
			Renderer::getRenderer().setTargeted(true);
			if (warningSound.getStatus() != warningSound.Playing)
				warningSound.play();
		}
		else {
			Renderer::getRenderer().setTargeted(false);
			warningSound.stop();
		}
		auto mv = Input::mouseMov();

		double fbt = 0.05;

		auto control1 = [dt](int key1) {
			if (Input::isKeyDown(key1)) {
				return 1.f;
			}
			return 0.f;
		};
		auto control2 = [dt](int key1, int key2) {
			if (Input::isKeyDown(key1)) {
				return -1.f;
			}
			else if (Input::isKeyDown(key2)) {
				return 1.f;
			}
			return 0.f;
		};

		flight->roll += mv.x * 0.005;
		flight->roll = glm::clamp(flight->roll, -1.f, 1.f);
		flight->pitch += mv.y * 0.005;
		flight->pitch = glm::clamp(flight->pitch, -1.f, 1.f);

		float roll = control2(GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
		float pitch = control2(GLFW_KEY_UP, GLFW_KEY_DOWN);
		float yaw = control2(GLFW_KEY_D, GLFW_KEY_A);

		float throttle = control1(GLFW_KEY_W);
		float brake = control1(GLFW_KEY_S);

		if (Input::gamepad_present()) {
			roll = Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_X);
			pitch = Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_Y);
			yaw = -Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_X);
			throttle = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1) / 2.f);
			brake = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1) / 2.f);
		}

		if (throttle > 0.9f && !BPHold) {
			BP++;
			BPHold = true;
			boostTimer.restart();
		}
		else if (BPHold && throttle < 0.1f) {
			BPHold = false;
			boostTimer.restart();
		}
		if (boostTimer.elapsed() > 0.2f && throttle < 0.1f || flight->burnOut) {
			BP = 0;
		}

		if (BP >= 2) {
			//std::cout << "AFTERBURNER ACTIVE!\n";
			flight->afterBurner = true;
		}
		else {
			flight->afterBurner = false;
		}

		float new_drift = 0.f;
		float d1 = 0;
		float d2 = 0;

		if (Input::gamepad_present()) {
			d1 = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1) / 2.f);
			d2 = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1) / 2.f);
		}
		float driftFactor = 0.f;
		if (d1 > 0.5f && d2 > 0.5f) {
			driftFactor = d1 + d2 - 1.f;
		}
		if (Input::isKeyDown(GLFW_KEY_SPACE)) {
			driftFactor = 1.0f;
		}

		flight->i_drift = driftFactor;

		if (player->flyByWire) {
			glm::vec3 ABS_up = glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 front = transform->orientation * glm::vec3(0.f, 0.f, 1.f);
			glm::vec3 fixed_left = glm::cross(ABS_up, front);
			glm::vec3 fixed_up = glm::cross(fixed_left, front);
			glm::vec3 flyToPos = transform->pos + (fixed_left * yaw) + (fixed_up * -pitch) + front;
			glm::vec3 input = SAIB::flyTo(transform->pos, transform->orientation, flyToPos);
			flight->setInput(input);
			std::cout << roll << std::endl;
			if (roll > 0.1f || roll < -0.1f) {
				flight->i_roll = roll;
			}
		}
		else {

			flight->i_pitch = pitch;
			flight->i_roll = roll;
			flight->i_yaw = yaw;
		}

		flight->i_throttle = throttle;
		flight->i_airBrake = brake;




		/**********************
		** outside map logic **
		***********************/
		ComponentHandle<Terrain> terrain;
		for (Entity tEntity : es.entities_with_components(terrain)) {

			player->isOutside = terrain->hmptr->isOutside(transform->pos);
			if (!player->isOutside) {
				player->outsideTimer.restart();
			}

			double outsideTime = 10.0;

			double elapsed = player->outsideTimer.elapsed();
			if (elapsed >= outsideTime) {

				auto health = entity.component<HealthComponent>();
				if (health) {
					health->health = -1;
				} else {
					std::cout << "[WARNING] Player entity does not have HealthComponent\n";
					// Nnonoononon
				}
			}

			player->outsideTimeLeft = outsideTime - elapsed;
			player->outsideTimeLeft = glm::max(player->outsideTimeLeft, 0.0);
		}




		/***********
		** flares **
		***********/
		player->flareTime += dt;
		if (Input::isKeyPressed(GLFW_KEY_F) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT)) {
			
			if (player->flareTime > player->coolDown) {
				player->flareTime = 0;
				player->flareAccum = 0;
				player->flareActive = true;
			}
		}
		if (player->flareTime  > 1.f) {
			player->flareActive = false;
		}

		if (player->flareActive) {
			player->flareAccum += dt;

			float emitCoolDown = 1.0 / 10.0;

			while (player->flareAccum > 0) {

				spawnFlare(es.create(), glm::vec3(1, 2, 0), transform.get(), physics.get(), events);
				spawnFlare(es.create(), glm::vec3(-1, 2, 0), transform.get(), physics.get(), events);

				player->flareAccum -= emitCoolDown;
			}
		}


		/***************
		** aim assist **
		***************/
		glm::vec3 forward = transform->orientation * glm::vec3(0, 0, 1);
		player->hasTarget = false;
		double bestScore = -1;
		ComponentHandle<Target> target;
		ComponentHandle<Transform> tTrans;
		ComponentHandle<FactionEnemy> factionEnemy;
		for (Entity enemy : es.entities_with_components(target, tTrans, factionEnemy)) {
			glm::vec3 dir = tTrans->pos - transform->pos;
			double score = dot(normalize(dir), forward) / length(dir); 
			if (score > bestScore && length(dir) < 400.f) {
				bestScore = score;
				player->hasTarget = true;
				player->target = enemy;
			}
		}


		
		glm::vec3 pos = transform->pos;
		//std::cout << "player pos: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";
	}
}

void spawnFlare(Entity flare, glm::vec3 dir, Transform* transform, Physics* physics, EventManager & events) {
	flare.assign<Transform>(transform->pos);

	float speed = 10.f;
	glm::vec3 vel = physics->velocity + speed * (transform->orientation * normalize(dir));

	flare.assign<Physics>(10,  10, vel, glm::vec3(0));
	flare.assign<Target>(15.0, FACTION_PLAYER);

	flare.assign<BurstSoundComponent>(*AssetLoader::getLoader().getSoundBuffer("flare"), transform->pos, true, 50.f, 1.1f, 1);

	auto handle = flare.assign<ParticleComponent>();

	events.emit<AddParticleEvent>(FLARE, handle);

	flare.assign<LifeTimeComponent>(5.f);

	
}

void PlayerSystem::configure(EventManager &eventManager) {
	eventManager.subscribe<PauseEvent>(*this);
}

void PlayerSystem::receive(const PauseEvent &event) {
	if (event.pause) {
		if (warningSound.getStatus() == sf::Sound::Playing)
			warningSound.pause();
	}
	else {
		if (warningSound.getStatus() == sf::Sound::Paused)
			warningSound.play();
	}
}