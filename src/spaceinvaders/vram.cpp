#include "vram.hpp"
#include "utility/pixel.hpp"

void c_vram::render()
{
	for (std::int32_t x = 0; x < BYTES_MAX_X; x++)
	{
		for (std::int32_t y = 0; y < BYTES_MAX_Y; y++)
		{
			for (std::uint8_t b = 0; b < 7; b++)
			{
				std::uint8_t pixel = (this->vram[x * BYTES_MAX_X + y] >> b) & 1;

				if (pixel)
				{
					utility::set_pixel(x * 8 + b, y * 8, 255, 255, 255, 255, this->renderer);
				}
			}
		}
	}
}

void c_vram::map_pointer(std::uint8_t* ram)
{
	this->vram = &ram[VRAM_START_ADDRESS];
}