#include "cpu/8080.hpp"

int main(int argc, char** argv)
{
	c_8080 i8080{ "8080exer.rom" };

	if (i8080.image_name == "cpudiag.rom" ||
		i8080.image_name == "8080exer.rom" ||
		i8080.image_name == "test.rom")
	{
		i8080.is_debug_image = true;
	}

	if (i8080.is_load_success())
		i8080.emulate();

	return 0;
}