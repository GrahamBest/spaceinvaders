#include <iostream>
#include <array>

constexpr auto VRAM_SIZE = 0x1C00;

/* vram is broken down by essentially look at the huge
* data map in space invaders from byte 0x2000 to byte 0x3FFF (0x3FFF - 0x2000 = 0x1C00)
* bit by bit where each bit cooresponds to each pixel 
* in the 0x1C00 byte long vram
*/
class c_vram
{
public:

	std::array<std::uint8_t, VRAM_SIZE> vram;
private:

};