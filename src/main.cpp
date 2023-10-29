#include "cpu/8080.hpp"

#undef main

int main(int argc, char** argv)
{
	c_8080 i8080{ "spaceinvaders.rom" };

	if (i8080.image_name == "cpudiag.rom" ||
		i8080.image_name == "8080exer.rom" ||
		i8080.image_name == "test.rom" ||
		i8080.image_name == "8080exm.rom")
	{
		i8080.is_debug_image = true;

		/* some intel 8080 rom images for CP/M use a different base
		* 0x100 is the base address of the rom image for debug images
		* we set the base to this so we can fix calculations to fix our
		* 0x0000 base.
		*/

		i8080.base = 0x100;
	}

	if (i8080.is_load_success())
		i8080.emulate();

	return 0;
}