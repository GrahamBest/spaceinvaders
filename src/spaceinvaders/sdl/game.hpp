#pragma once

#include <SDL.h>
#include <string>
#include "../vram.hpp"

class c_game
{
public:
	c_game(const std::string& name)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			std::printf("FATAL ERROR: Failed to initialize SDL!\n");
			return;
		}

		SDL_Init(SDL_INIT_VIDEO);
		SDL_CreateWindowAndRenderer(1080, 780, 0, &this->window, &this->renderer);

		SDL_RenderSetScale(this->renderer, 4, 4);
	}

	void update_window(SDL_Event& event)
	{
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			std::exit(EXIT_SUCCESS);

		SDL_RenderPresent(this->get_renderer_ptr());
	}

	SDL_Window* get_window_ptr()
	{
		return this->window;
	}

	SDL_Renderer* get_renderer_ptr()
	{
		return this->renderer;
	}
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
};