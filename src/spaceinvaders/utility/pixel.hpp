#pragma once

#include <iostream>
#include <SDL.h>

namespace utility
{
	void set_pixel(std::int32_t x, std::int32_t y, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a, SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawPoint(renderer, x, y);
	}
}