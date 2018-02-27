#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"
#include "planepreset.h"
#include "weaponpreset.h"


//enum PAGE { PLANES, WEAPONS, SKINS };

#define NO_WEAPON -1

enum Color { red, green, blue };

enum Page { planes, weapons, skins };

class LoadoutState : public EngineState {
private:
	FullscreenImage bg;
	ButtonHandler bHandler;
	ButtonHandler planesBHandler;
	ButtonHandler weaponSlotsBHandler;
	ButtonHandler weaponsBHandler;
	ButtonHandler skinsBHandler;
	unsigned int selected;
	unsigned int selectedW;
	unsigned int selectedSkin = 0;
	int page = planes;
	std::vector<PlanePreset> planePresets;
	std::vector<WeaponPreset> weaponPresets;
	std::vector<int> pickedWeapons;
	bool picking = false, pagePicked = false, planePicked = false;

	Timer timer;
	entityx::EntityX ex;
	void updatePreview();
public:
	virtual ~LoadoutState() {}
	virtual void init();
	virtual void update(double dt);
	void startMenu();
	void changePlane(unsigned int selected);
	void changeWeapon(unsigned int selected);
	void changeSkin(unsigned int selected);
	void changePage(int page);
	void pickWeapon(unsigned int selected);
	void saveLoadout();
	void clearPicks();
};