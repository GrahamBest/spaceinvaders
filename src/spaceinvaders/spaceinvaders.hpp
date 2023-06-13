#pragma once

#include "vram.hpp"
#include "sdl/game.hpp"
#include <iostream>

class c_spaceinvaders
{
public:
	c_spaceinvaders()
	{
		this->game_running = true;
	}

	void update(SDL_Event& event);

	bool game_running{ true };
	c_game game{ "Intel 8080 Space Invaders" };
	c_vram vram{this->game.get_window_ptr(), this->game.get_renderer_ptr()};
private:

};