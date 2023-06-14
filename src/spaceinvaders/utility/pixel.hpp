#pragma once

#include <iostream>
#include <raylib.h>

namespace utility
{
	void set_pixel(std::int32_t x, std::int32_t y, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	{
		DrawPixel(x, y, Color{ r, g, b, a });
	}
}