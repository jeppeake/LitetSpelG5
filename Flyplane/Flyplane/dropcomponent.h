#pragma once


struct DropComponent {
	//enum TypeOfDrop { Micromissile, Rocketpod, Fishrod, Rodfish, Bigboi, HAAM_166, SHAAM_200, Health, NrOfItems };
	enum TypeOfDrop { Ammo, Health, NrOfItems};
	//DropComponent(TypeOfDrop type = Health, float amount = 0) : amount(amount), type(type) {}
	DropComponent(TypeOfDrop type) : type(type) {}

	TypeOfDrop type;
	//float amount;
};