#include "vram.hpp"
#include "../cpu/8080.hpp"
#include "SDL.h"

bool vblank = true;
void c_vram::render(c_8080* ptr)
{
	std::int16_t i = vblank ? 0 : HALF_SCREEN;

	std::uint8_t b = 0;

	/* interrupts get sent out after each half of the screen has been rendered */
	for (std::int32_t y = !vblank ? 0x70 : 0; y < PIXEL_MAX_Y; y++)
	{
		for (std::int32_t x = 0; x < PIXEL_MAX_X; x++)
		{
			this->pixels[(PIXEL_MAX_X - x - 1) * PIXEL_MAX_Y + y] = ((this->vram[i] >> b) & 1) * 0xFF;

			b++;
			if (b == 8)
			{
				b = 0;

				i++;
			}

			if (i == HALF_SCREEN && vblank) { //draw first half
				vblank = false;
				ptr->generate_interrupt(ISR_RST0);
				return;
			}
		}
	}

	vblank = true;
	ptr->generate_interrupt(ISR_RST2);
	SDL_UpdateTexture(this->texture, NULL, this->pixels.get(), PIXEL_MAX_X * sizeof(std::uint8_t));
	SDL_RenderClear(this->renderer);
	SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
	SDL_RenderPresent(this->renderer);
}

void c_vram::map_pointer(std::uint8_t* ram)
{
	this->vram = &ram[VRAM_START_ADDRESS];
}