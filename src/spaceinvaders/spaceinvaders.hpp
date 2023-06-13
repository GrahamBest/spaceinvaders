#include "vram.hpp"
#include <iostream>

class c_spaceinvaders
{
public:
	c_spaceinvaders()
	{
		std::fill(this->vram.vram.begin(), this->vram.vram.end(), 0);
	}

	void render();

	c_vram vram{};
private:

};