#pragma once


struct DropComponent {
	enum TypeOfDrop { Micromissile, Rocketpod, Fishrod, Rodfish, Bigboi, Health, NrOfItems };
	DropComponent(float amount = 0, TypeOfDrop type = Health) : amount(amount), type(type) {}

	TypeOfDrop type;
	float amount;
};