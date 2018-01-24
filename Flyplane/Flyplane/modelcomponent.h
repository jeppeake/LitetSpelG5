#pragma once
#include "model.h"
struct ModelComponent
{
	ModelComponent(Model *mptr) : mptr(mptr) {}
	Model *mptr;
};