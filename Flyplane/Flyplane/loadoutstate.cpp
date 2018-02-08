#include "loadoutstate.h"
#include "menustate.h"
#include "backtomenuaction.h"
#include "window.h"
#include "planepreset.h"
#include "changepageaction.h"
#include "changeplaneaction.h"
#include "changeweaponaction.h"
#include "pickweaponaction.h"
#include "saveloadoutaction.h"
#include <stdio.h>
#include "entityx\Entity.h"
#include "modelcomponent.h"
#include "transform.h"
#include "weapon.h"
#include "equipment.h"
#include "rendersystem.h"
#include "rotatepreviewsystem.h"

entityx::Entity entityp;
Transform planetrans;


void LoadoutState::updatePreview() {
	Transform oldTrans;
	glm::vec3 pos(-3, -2, 0);
	glm::quat orien(1, 0, 0, 0);
	oldTrans.pos = pos;
	if(entityp.has_component<Transform>())
		oldTrans = *entityp.component<Transform>().get();

	entityp.destroy();
	entityp = ex.entities.create();
	// ---	PLAYER	---
	entityp.assign<Transform>(oldTrans.pos, normalize(oldTrans.orientation));
	entityp.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
	entityp.assign<PreviewComponent>();

	std::vector<Weapon> weapons;
	std::vector<Weapon> pweapons;

	for (int i = 0; i < this->planePresets[this->selected].wepPos.size(); i++) {
		if (this->pickedWeapons[i] != NO_WEAPON) {
			glm::vec3 pos = this->planePresets[this->selected].wepPos[i];
			WeaponPreset pr = this->weaponPresets[this->pickedWeapons[i]];
			WeaponStats stats = WeaponStats(pr.ammo, pr.lifetime, pr.speed, pr.mass, pr.cooldown, pr.infAmmo);
			weapons.emplace_back(stats, AssetLoader::getLoader().getModel(pr.name), AssetLoader::getLoader().getModel(pr.projModel), pos, glm::vec3(pr.scale), glm::vec3(pr.projScale), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
		}
	}
	entityp.assign <Equipment>(pweapons, weapons);
}

void LoadoutState::init() {
	entityp = ex.entities.create();
	Camera camera;
	Transform trans;
	trans.pos = glm::vec3(0, 0, -10);
	camera.setTransform(trans);
	Renderer::getRenderer().setCamera(camera);
	ex.systems.add<RenderSystem>();
	ex.systems.add<RotatePreviewSystem>();
	ex.systems.configure();

	timer.restart();

	AssetLoader::getLoader().loadSound("Assets/Sound/button.wav", "buttonsound");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonforward.wav", "buttonforward");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonback.wav", "buttonback");

	bHandler.addButton(new Button("Back to menu", glm::vec2(100, 600), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new BackToMenuAction(this), "buttonback"));
	bHandler.addButton(new Button("Save loadout", glm::vec2(400, 600), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new SaveLoadoutAction(this), "buttonback"));
	bHandler.addButton(new Button("Planes", glm::vec2(50, 50), glm::vec2(150, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangePageAction(this, planes), "buttonforward"));
	bHandler.addButton(new Button("Weapons", glm::vec2(250, 50), glm::vec2(150, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangePageAction(this, weapons), "buttonforward"));
	bHandler.addButton(new Button("Skins", glm::vec2(450, 50), glm::vec2(150, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangePageAction(this, skins), "buttonforward"));
	
	PlanePreset pr;
	pr.load("assets/Presets/Planes/MIG-212A.txt");
	planePresets.push_back(pr);
	pr = PlanePreset();
	pr.load("assets/Presets/Planes/TU-101.txt");
	planePresets.push_back(pr);

	WeaponPreset wp;
	wp.load("assets/Presets/Weapons/missile.txt");
	weaponPresets.push_back(wp);
	wp = WeaponPreset();
	wp.load("assets/Presets/Weapons/missile2.txt");
	weaponPresets.push_back(wp);
	wp = WeaponPreset();
	wp.load("assets/Presets/Weapons/missile3.txt");
	weaponPresets.push_back(wp);

	glm::vec2 pPos = glm::vec2(50, 150);
	for (int i = 0; i < planePresets.size(); i++) {
		planesBHandler.addButton(new Button(planePresets[i].name, pPos + glm::vec2(0, i*(40)), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangePlaneAction(this, i), "buttonforward"));
	}

	pPos = glm::vec2(450, 150);
	for (int i = 0; i < weaponPresets.size(); i++) {
		weaponsBHandler.addButton(new Button(weaponPresets[i].name, pPos + glm::vec2(0, i*(40)), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new PickWeaponAction(this, i), "buttonforward"));
	}
	
}

void LoadoutState::startMenu() {
	this->changeState(new MenuState());
}

void LoadoutState::changePlane(unsigned int selected)
{
	clearPicks();
	this->selected = selected;
	glm::vec2 pPos = glm::vec2(50, 150);
	weaponSlotsBHandler.clearButtons();
	for (int i = 0; i < planePresets[selected].wepPos.size(); i++) {
		weaponSlotsBHandler.addButton(new Button("Empty", pPos + glm::vec2(0, i*(40)), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangeWeaponAction(this, i), "buttonforward"));
		pickedWeapons.push_back(NO_WEAPON);
	}
	updatePreview();
}

void LoadoutState::changeWeapon(unsigned int selected)
{
	if (this->selectedW == selected && picking)
		picking = false;
	else
		picking = true;
	this->selectedW = selected;
	for (Button* button : weaponSlotsBHandler.buttons) {
		button->color = glm::vec3(1, 1, 1);
	}
	if(picking)
		weaponSlotsBHandler.buttons[this->selectedW]->color = weaponSlotsBHandler.buttons[this->selectedW]->hcolor;
}

void LoadoutState::changePage(int page)
{
	this->page = page;
}

void LoadoutState::pickWeapon(unsigned int selected)
{
	this->pickedWeapons[this->selectedW] = selected;
	this->weaponSlotsBHandler.buttons[this->selectedW]->bText = this->weaponPresets[selected].name;
	picking = false;
	for (Button* button : weaponSlotsBHandler.buttons) {
		button->color = glm::vec3(1, 1, 1);
	}
	updatePreview();
}

void LoadoutState::saveLoadout()
{
	remove("loadout.txt");
	ofstream outputFile("loadout.txt");
	outputFile << this->planePresets[this->selected].file << "\n";
	for (int i = 0; i < this->planePresets[this->selected].wepPos.size(); i++) {
		outputFile << this->weaponPresets[this->pickedWeapons[i]].file << "\n";
	}
}

void LoadoutState::clearPicks()
{
	for (int i = 0; i < pickedWeapons.size(); i++) {
		pickedWeapons[i] = NO_WEAPON;
	}
}

void LoadoutState::update(double dt) {
	glClearColor(33.0 / 255, 33.0 / 255, 33.0 / 255, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ex.systems.update<RenderSystem>(dt);
	ex.systems.update<RotatePreviewSystem>(dt);
	AssetLoader::getLoader().getBigtext()->drawText("LOADOUT", glm::vec2(100, Window::getWindow().size().y - 50), glm::vec3(1, 1, 1), 1);

	glm::vec2 infoPos = glm::vec2(Window::getWindow().size().x - 400, Window::getWindow().size().y - 50);
	double infoScale = 0.4;
	double step = 30;

	AssetLoader::getLoader().getMenutext()->drawText(planePresets[selected].name, infoPos, glm::vec3(1, 1, 1), 1);
	AssetLoader::getLoader().getText()->drawText("Normal speed: " + std::to_string(planePresets[selected].normalspeed) , infoPos - glm::vec2(0, step), glm::vec3(1, 1, 1), infoScale);
	AssetLoader::getLoader().getText()->drawText("Boost speed: " + std::to_string(planePresets[selected].boostspeed), infoPos - glm::vec2(0, step*2), glm::vec3(1, 1, 1), infoScale);
	AssetLoader::getLoader().getText()->drawText("Breakforce: " + std::to_string(planePresets[selected].breakforce), infoPos - glm::vec2(0, step*3), glm::vec3(1, 1, 1), infoScale);
	AssetLoader::getLoader().getText()->drawText("Turn rate: " + std::to_string(planePresets[selected].turnrate), infoPos - glm::vec2(0, step*4), glm::vec3(1, 1, 1), infoScale);
	AssetLoader::getLoader().getText()->drawText("Weapon slots: " + std::to_string(planePresets[selected].wepPos.size()), infoPos - glm::vec2(0, step*5), glm::vec3(1, 1, 1), infoScale);

	bHandler.drawButtons();
	bHandler.handleButtonClicks();
	switch(this->page) {
	case planes:
		planesBHandler.drawButtons();
		planesBHandler.handleButtonClicks();
		break;
	case weapons:
		weaponSlotsBHandler.drawButtons();
		weaponSlotsBHandler.handleButtonClicks();
		if (picking) {
			weaponsBHandler.drawButtons();
			weaponsBHandler.handleButtonClicks();
		}
		break;
	}
	
}