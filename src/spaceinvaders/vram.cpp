#include "vram.hpp"
#include "utility/pixel.hpp"

void c_vram::render()
{
	for (std::int32_t x = 0; x < 256; x++)
	{
		for (std::int32_t y = 0; y < 244; y++)
		{
			for (std::uint8_t b = 0; b < 7; b++)
			{
				std::uint8_t pixel = (this->vram[x * BYTES_MAX_X + y] >> b) & 1;

				utility::set_pixel(x * 8 + b, y * 8 + b, 255, 255, 255, 255);
			}
		}
	}
}

void c_vram::map_pointer(std::uint8_t* ram)
{
	this->vram = &ram[VRAM_START_ADDRESS];
}