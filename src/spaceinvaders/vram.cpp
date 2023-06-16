#include "vram.hpp"
#include "../cpu/8080.hpp"
#include "utility/pixel.hpp"

void c_vram::render(c_8080* ptr)
{
	bool vblank = true;
	std::int16_t i = 0;

	for (std::int32_t y = !vblank ? 0x70 : 0; y < PIXEL_MAX_Y; y++)
	{
		for (std::int32_t x = 0; x < PIXEL_MAX_X; x++)
		{
			for (std::uint8_t b = 0; b < 8; b++)
			{
				this->pixels[(PIXEL_MAX_X - x - 1) * PIXEL_MAX_Y + y] = (this->vram[i] >> b) & 1;

				if (this->pixels[(PIXEL_MAX_X - x - 1) * PIXEL_MAX_Y + y])
					utility::set_pixel(PIXEL_MAX_X - x + b, y, 255, 255, 255, 255);
			}

			i++;

			if (i == 0xE00 && vblank)
			{
				vblank = false;
				ptr->interrupt_handler.generate_interrupt(ISR_RST1, ptr);
			}
		}
	}

	ptr->interrupt_handler.generate_interrupt(ISR_RST2, ptr);
}

void c_vram::map_pointer(std::uint8_t* ram)
{
	this->vram = &ram[VRAM_START_ADDRESS];
}