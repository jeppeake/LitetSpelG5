#pragma once


struct DropComponent {
	enum TypeOfDrop { Ammo, Health, NrOfItems};
	DropComponent(TypeOfDrop type) : type(type) {}

	TypeOfDrop type;
};