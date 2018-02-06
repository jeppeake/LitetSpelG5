#include "button.h"
#include "assetloader.h"
#include "window.h"

void Button::drawButton() {
	glm::vec2 dpos = glm::vec2(pos.x, Window::getWindow().size().y - pos.y);
	AssetLoader::getLoader().getMenutext()->drawText(bText, glm::vec2(dpos.x, dpos.y - 48.f), dcolor, 1);
}