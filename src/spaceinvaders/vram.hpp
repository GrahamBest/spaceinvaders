#include <iostream>
#include <array>
#include <bitset>

constexpr auto VRAM_SIZE = 0x1C00;
constexpr auto VRAM_START_ADDRESS = 0x2400;
constexpr auto PIXEL_AMOUNT = 0xE000;
constexpr auto PIXEL_MAX_X = 0xFF;
constexpr auto PIXEL_MAX_Y = 0xE0;

/* vram is broken down by essentially look at the huge
* data map in space invaders from byte 0x4000 to byte 0x3FFF (0x3FFF - 0x2000 = 0x1C00)
* bit by bit where each bit cooresponds to each pixel
* in the 0x1C00 byte long vram
*/

/* class implementation
* that abstracts the mapping
* from pixels to bits
*/ 

class pixelbyte_exception : public std::exception
{
public:
	pixelbyte_exception(const std::string& what)
	{	msg = what; }

	std::string msg;

	const char* what() const override
	{
		return msg.c_str();
	}
};

class c_pixelbyte
{
public:
	void set(std::uint8_t pos, bool value)
	{
		if (pos >= 0 && pos <= 7 && value == 1 || value == 0)
		{
			this->pixel_map[pos] = value;
		}
		else
		{
			throw pixelbyte_exception{ "Pixel byte non boolean value or index is out of range" };
		}
	}

	void clear(std::uint8_t pos)
	{
		if (pos >= 0 && pos <= 7)
		{
			this->pixel_map[pos] = 0;
		}
		else
		{
			throw pixelbyte_exception{ "Pixel index is out of range" };
		}
	}

	bool is_set(std::uint8_t pos)
	{
		if (pos >= 0 && pos <= 7)
		{
			if (this->pixel_map[pos])
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		throw pixelbyte_exception{ "Pixel index is out of range" };
	}

	std::bitset<8> pixel_map;
private:

};

class c_vram
{
public:
	c_vram()
	{
		std::fill(this->vram.begin(), this->vram.end(), 0);
	}

	void render();
	void copy_from_main(const std::uint8_t* ram);

	std::array<c_pixelbyte, VRAM_SIZE> vram;
private:

};