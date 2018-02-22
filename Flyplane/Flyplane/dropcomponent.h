#pragma once


struct DropComponent {
	static enum TypeOfDrop { Weapon, Health };
	DropComponent(float amount = 0, TypeOfDrop type = Health) : amount(amount), type(type) {}

	TypeOfDrop type;
	float amount;
};