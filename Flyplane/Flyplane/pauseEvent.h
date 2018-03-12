#pragma once

struct PauseEvent {
	bool pause;
	entityx::EntityX& ex;

	PauseEvent(bool pause, entityx::EntityX& ex) : pause(pause), ex(ex) {}
};