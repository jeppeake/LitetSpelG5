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
#include "openpreorderaction.h"
#include <stdio.h>
#include "entityx\Entity.h"
#include "modelcomponent.h"
#include "transform.h"
#include "weapon.h"
#include "equipment.h"
#include "rendersystem.h"
#include "rotatepreviewsystem.h"
#include "changeskinaction.h"
#include "preorderstate.h"
#include <fstream>
#include <iostream>
#include <experimental\filesystem>
#include "musicmanager.h"
namespace fs = std::experimental::filesystem;

entityx::Entity entityp;
Transform planetrans;
void LoadoutState::updatePreview() {
	Transform oldTrans;
	glm::vec3 pos(-1.5, -1, 0);
	glm::quat orien(1, 0, 0, 0);
	oldTrans.pos = pos;
	if(entityp.has_component<Transform>())
		oldTrans = *entityp.component<Transform>().get();

	entityp.destroy();
	entityp = ex.entities.create();
	// ---	PLAYER	---
	entityp.assign<Transform>(oldTrans.pos, normalize(oldTrans.orientation));
	entityp.assign <ModelComponent>(AssetLoader::getLoader().getModel(planePresets[this->selected].name));
	entityp.assign<PreviewComponent>();

	std::vector<Weapon> weapons;
	std::vector<Weapon> pweapons;
	std::vector<Turret> turrets;
	//primary load
	std::cout << "Weapon: " << this->planePresets[this->selected].weapon << " : " << this->planePresets[this->selected].turretWeapon << "\n";
	if (this->planePresets[this->selected].weapon != "nan") {
		if (this->planePresets[this->selected].turretWeapon) {//turret
			TurretPreset TP;
			TP.load(this->planePresets[this->selected].weapon);
			//std::cout << "Loaded: " << this->planePresets[this->selected].weapon << "\n";
			turrets.emplace_back(TP.getTurret());
		}
		else {//primary
			WeaponPreset PW;
			PW.load(this->planePresets[this->selected].weapon);
			//std::cout << "Loaded: " << this->planePresets[this->selected].weapon << "\n";
			WeaponStats stats = WeaponStats(PW.ammo, PW.lifetime, PW.speed, PW.mass, PW.cooldown, PW.infAmmo);
			pweapons.emplace_back(stats, AssetLoader::getLoader().getModel(PW.name), AssetLoader::getLoader().getModel(PW.projModel), PW.extraOffset, glm::vec3(PW.scale), glm::vec3(PW.projScale), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
		}
	}
	//turret load
	for (int i = 0; i < this->planePresets[this->selected].turretFiles.size(); i++) {
		TurretPreset TP;
		TP.load(this->planePresets[this->selected].turretFiles[i]);
		Turret turret = TP.getTurret();
		turret.placement.offset = this->planePresets[this->selected].turretPositions[i];
		glm::vec3 ori = this->planePresets[this->selected].turretFronts[i];
		std::cout << ori.x << " : " << ori.y << " : " << ori.z << "\n";
		turret.placement.orientation = glm::quat(glm::radians(this->planePresets[this->selected].turretOrientations[i]));
		turret.placement.front = this->planePresets[this->selected].turretFronts[i];
		turrets.emplace_back(turret);
	}
	//secondary load
	for (int i = 0; i < this->planePresets[this->selected].wepPos.size(); i++) {
		if (this->pickedWeapons[i] != NO_WEAPON) {
			glm::vec3 pos = this->planePresets[this->selected].wepPos[i];
			WeaponPreset pr = this->weaponPresets[this->pickedWeapons[i]];
			WeaponStats stats = WeaponStats(pr.ammo, pr.lifetime, pr.speed, pr.mass, pr.cooldown, pr.infAmmo);
			weapons.emplace_back(stats, AssetLoader::getLoader().getModel(pr.name), AssetLoader::getLoader().getModel(pr.projModel), pos + pr.extraOffset, glm::vec3(pr.scale), glm::vec3(pr.projScale), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
		}
	}
	entityp.assign <Equipment>(pweapons, weapons, turrets);
}

void LoadoutState::init() {
	bg.load("assets/Textures/loadoutbg.png");

	entityp = ex.entities.create();
	Camera camera;
	Transform trans;
	trans.pos = glm::vec3(0, 0, -5);
	camera.setTransform(trans);
	Renderer::getRenderer().setCamera(camera);
	ex.systems.add<RenderSystem>();
	ex.systems.add<RotatePreviewSystem>();
	ex.systems.configure();

	Renderer::getRenderer().setHeightmap(nullptr);
	Renderer::getRenderer().setDrawShadows(false);
	Renderer::getRenderer().setDrawClouds(false);
	ex.systems.system<RenderSystem>()->playing = false;


	timer.restart();

	AssetLoader::getLoader().loadSound("Assets/Sound/button.wav", "buttonsound");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonforward.wav", "buttonforward");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonback.wav", "buttonback");

	bHandler.addButton(new Button("PLANES", glm::vec2(50, 50), glm::vec2(150, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangePageAction(this, planes), "buttonforward"));
	bHandler.addButton(new Button("WEAPONS", glm::vec2(250, 50), glm::vec2(150, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangePageAction(this, weapons), "buttonforward"));
	bHandler.addButton(new Button("SKINS", glm::vec2(480, 50), glm::vec2(150, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangePageAction(this, skins), "buttonforward"));
	bHandler.addButton(new Button("BACK TO MENU", glm::vec2(50, Window::getWindow().size().y - 90), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new BackToMenuAction(this), "buttonback"));
	bHandler.addButton(new Button("SAVE LOADOUT", glm::vec2(350, Window::getWindow().size().y - 90), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new SaveLoadoutAction(this), "buttonforward"));
	bHandler.addButton(new Button("Activate preorder", glm::vec2(650, Window::getWindow().size().y - 90), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new OpenPreorderAction(this), "buttonforward"));
	
	//Load all planes in plane preset folder
	std::string path = "assets/Presets/Planes";
	for (auto & p : fs::directory_iterator(path)) {
		std::string curPath = p.path().string();
		PlanePreset pr;
		pr.preorder = this->preorder;
		pr.load(curPath);
		planePresets.push_back(pr);
	}

	//load all weapons in weapon preset folder
	path = "assets/Presets/Weapons";
	for (auto & p : fs::directory_iterator(path)) {
		std::string curPath = p.path().string();
		WeaponPreset pr;
		pr.load(curPath);
		weaponPresets.push_back(pr);
	}

	//place buttons
	glm::vec2 pPos = glm::vec2(50, 150);
	for (int i = 0; i < planePresets.size(); i++) {
		planesBHandler.addButton(new Button(planePresets[i].name, pPos + glm::vec2(0, i*(40)), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangePlaneAction(this, i), "buttonforward"));
	}

	pPos = glm::vec2(450, 150);
	for (int i = 0; i < weaponPresets.size(); i++) {
		weaponsBHandler.addButton(new Button(weaponPresets[i].name, pPos + glm::vec2(0, i*(40)), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new PickWeaponAction(this, i), "buttonback"));
	}
	
	bHandler.buttons[this->page]->color = bHandler.buttons[this->page]->hcolor;

	if (preorder) {
		sb.loadFromFile("assets/Sound/sellout.wav");
		selloutSound.setBuffer(sb);
		selloutSound.play();
	}
}

void LoadoutState::startMenu() {
	this->changeState(new MenuState());
}

void LoadoutState::startPreorder()
{
	this->changeState(new PreorderState());
}

void LoadoutState::changePlane(unsigned int selected)
{
	clearPicks();
	this->selected = selected;
	glm::vec2 pPos = glm::vec2(50, 150);
	weaponSlotsBHandler.clearButtons();
	for (int i = 0; i < planePresets[selected].wepPos.size(); i++) {
		weaponSlotsBHandler.addButton(new Button("EMPTY", pPos + glm::vec2(0, i*(40)), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangeWeaponAction(this, i), "buttonforward"));
		pickedWeapons.push_back(NO_WEAPON);
	}
	skinsBHandler.clearButtons();
	for (int i = 0; i < planePresets[selected].textureNames.size(); i++) {
		skinsBHandler.addButton(new Button(planePresets[selected].textureNames[i], pPos + glm::vec2(0, i*(40)), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new ChangeSkinAction(this, i), "buttonforward"));
	}
	updatePreview();
	for (Button* button : planesBHandler.buttons) {
		button->color = glm::vec3(1, 1, 1);
	}
	planePicked = true;
	planesBHandler.buttons[this->selected]->color = planesBHandler.buttons[this->selected]->hcolor;
	changeSkin(0);
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

void LoadoutState::changeSkin(unsigned int selected)
{
	selectedSkin = selected;
	entityp.component<ModelComponent>().get()->mptr->texture = *AssetLoader::getLoader().getTexture(this->planePresets[this->selected].textureNames[selected]);
	for (Button* button : skinsBHandler.buttons) {
		button->color = glm::vec3(1, 1, 1);
	}
	skinsBHandler.buttons[this->selectedSkin]->color = skinsBHandler.buttons[this->selectedSkin]->hcolor;
}

void LoadoutState::changePage(int page)
{
	this->page = page;
	for (Button* button : bHandler.buttons) {
		button->color = glm::vec3(1, 1, 1);
	}
	bHandler.buttons[this->page]->color = bHandler.buttons[this->page]->hcolor;
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
	if (planePicked) {
		remove("loadout.txt");
		ofstream outputFile("loadout.txt");
		outputFile << this->planePresets[this->selected].file << "\n";
		for (int i = 0; i < this->planePresets[this->selected].wepPos.size(); i++) {
			if (this->pickedWeapons[i] != NO_WEAPON)
				outputFile << this->weaponPresets[this->pickedWeapons[i]].file << "\n";
			else
				outputFile << 0 << "\n";
		}
		outputFile << this->selectedSkin << "\n";
	}}

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

	

	AssetLoader::getLoader().getBigtext()->drawText("LOADOUT", glm::vec2(50, Window::getWindow().size().y - 50), glm::vec3(1, 1, 1), 1);

	glm::vec2 infoPos = glm::vec2(Window::getWindow().size().x - 300, Window::getWindow().size().y - 300);
	double infoScale = 0.4;
	double step = 30;

	if (planePicked) {
		AssetLoader::getLoader().getBigtext()->drawText(planePresets[selected].name, infoPos, glm::vec3(1, 1, 1), 1);
		//AssetLoader::getLoader().getText()->drawText("Description: " + planePresets[selected].description, infoPos - glm::vec2(0, step * 1), glm::vec3(1, 1, 1), infoScale*0.75);
		AssetLoader::getLoader().getText()->drawText("Normal speed: " + std::to_string(planePresets[selected].normalspeed), infoPos - glm::vec2(0, step * 1), glm::vec3(1, 1, 1), infoScale);
		AssetLoader::getLoader().getText()->drawText("Boost speed: " + std::to_string(planePresets[selected].boostspeed), infoPos - glm::vec2(0, step * 2), glm::vec3(1, 1, 1), infoScale);
		AssetLoader::getLoader().getText()->drawText("Breakforce: " + std::to_string(planePresets[selected].breakforce), infoPos - glm::vec2(0, step * 3), glm::vec3(1, 1, 1), infoScale);
		AssetLoader::getLoader().getText()->drawText("Turn rate: " + std::to_string(planePresets[selected].turnrate), infoPos - glm::vec2(0, step * 4), glm::vec3(1, 1, 1), infoScale);
		AssetLoader::getLoader().getText()->drawText("Weapon slots: " + std::to_string(planePresets[selected].wepPos.size()), infoPos - glm::vec2(0, step * 5), glm::vec3(1, 1, 1), infoScale);
	}

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
	case skins:
		skinsBHandler.drawButtons();
		skinsBHandler.handleButtonClicks();
	}
	if (Input::isKeyPressed(GLFW_KEY_TAB))
		MusicManager::getMusicManager().changeSong();
	
}