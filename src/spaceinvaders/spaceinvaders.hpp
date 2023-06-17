#pragma once

#include "vram.hpp"
#include <iostream>

class c_spaceinvaders
{
public:
	c_spaceinvaders()
	{
		this->game_running = true;
	}

	void update();

	bool game_running{ true };
	c_vram vram;
private:

};