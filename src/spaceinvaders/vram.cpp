#include "vram.hpp"

void c_vram::render()
{

}

void c_vram::copy_from_main(std::uint8_t* ram)
{
	for (std::int32_t i = 0; i < VRAM_SIZE; i++)
	{
		this->vram[i] = ram[VRAM_START_ADDRESS + i];
	}
}