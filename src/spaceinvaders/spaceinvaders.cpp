#include "spaceinvaders.hpp"

void c_spaceinvaders::update(SDL_Event& event)
{
	this->vram.render();

	this->game.update_window(event);
}