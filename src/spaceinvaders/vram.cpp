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
				if (this->vram[x * BYTES_MAX_X + y].is_set(b))
				{
					utility::set_pixel(x * 8 + b, y * 8 + b, 255, 255, 255, 255, this->renderer);
				}
			}
		}
	}
}

void c_vram::copy_from_main(const std::uint8_t* ram)
{
	for (std::int32_t i = 0; i < VRAM_SIZE; i++)
	{
		this->vram[i].pixel_map = ram[VRAM_START_ADDRESS + i];
	}
}