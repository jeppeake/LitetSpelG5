#pragma once


struct DropComponent {
	enum TypeOfDrop { Micromissile, Rocketpod, Fishrod, Rodfish, Bigboi, Health, NrOfItems };
	DropComponent(TypeOfDrop type = Health, float amount = 0) : amount(amount), type(type) {}

	TypeOfDrop type;
	float amount;
};