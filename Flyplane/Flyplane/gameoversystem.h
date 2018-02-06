#pragma once

#include <entityx\entityx.h>
#include "playercomponent.h"

struct GameOver : public entityx::System<GameOver>, public entityx::Receiver<GameOver> {
	GameOver(PlayingState* state) : state(state) {}

	void configure(entityx::EventManager &eventManager) {
		//eventManager.subscribe<entityx::ComponentRemovedEvent<PlayerComponent>>(*this);
		eventManager.subscribe<entityx::EntityDestroyedEvent>(*this);
	}

	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) {}

	void receive(const entityx::EntityDestroyedEvent &entity) {
		if (entity.entity.has_component<PlayerComponent>()) {
			state->gameOver();
		}
		else {
			std::cout << "crash" << std::endl;
		}
	}

	PlayingState* state;
};