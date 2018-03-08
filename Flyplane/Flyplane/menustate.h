#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "optionsstate.h"
#include "fullscreenimage.h"

enum CameraState {
	panning,
	follow
};

struct CameraAngle {
	CameraAngle(glm::vec3 pos, glm::vec3 pandir) : pos(pos), pandir(pandir) {}
	glm::vec3 pos;
	glm::vec3 pandir;
};

class MenuState : public EngineState {
private:
	Camera camera;
	entityx::EntityX ex;
	ButtonHandler bHandler;
	void spawnEnemies(int nr);
	FullscreenImage vignette;
	Timer timer;
	CameraState cstate = panning;
	std::vector<CameraAngle> cangles;
	unsigned int curAngle = 0;
public:
	virtual ~MenuState();
	virtual void init();
	virtual void update(double dt);
	void startGame();
	void startOptions();
	void startLoadout();
	void startHighscore();
};