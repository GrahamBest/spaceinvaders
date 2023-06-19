#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <SDL.h>

constexpr auto VRAM_SIZE = 0x1C00;
constexpr auto HALF_SCREEN = 0xE00;
constexpr auto VRAM_START_ADDRESS = 0x2400;
constexpr auto PIXEL_AMOUNT = 0xE000;
constexpr auto PIXEL_MAX_X = 0xFF;
constexpr auto PIXEL_MAX_Y = 0xE0;

constexpr auto BYTES_MAX_X = 32;
constexpr auto BYTES_MAX_Y = 28;

constexpr auto WINDOW_WIDTH = 1200;
constexpr auto WINDOW_HEIGHT = 900;

/* vram is broken down by essentially look at the huge
* data map in space invaders from byte 0x4000 to byte 0x3FFF (0x3FFF - 0x2000 = 0x1C00)
* bit by bit where each bit cooresponds to each pixel
* in the 0x1C00 byte long vram
*/

class c_8080;

class c_vram
{
public:
	c_vram()
	{
		if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) 
		{
			std::printf("FATAL ERROR: FAILED TO INITIALIZE SDL!\n");
		}

		this->pixels = std::make_unique<std::uint8_t[]>(PIXEL_MAX_X * PIXEL_MAX_Y);

		this->window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
		this->renderer = SDL_CreateRenderer(this->window, -1, 0);
		this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STATIC, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	~c_vram()
	{
		SDL_DestroyWindow(this->window);
		SDL_DestroyRenderer(this->renderer);
		SDL_DestroyTexture(this->texture);
	}

	void render(c_8080* ptr);
	void map_pointer(std::uint8_t* ram);

	std::unique_ptr<std::uint8_t[]> pixels;
	std::uint8_t* vram = nullptr;
private:
	SDL_Window* window;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
};