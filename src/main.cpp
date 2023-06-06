#include "cpu/8080.hpp"

int main(int argc, char** argv)
{
	c_8080 i8080{ "cpudiag.rom" };

	if (i8080.is_load_success())
		i8080.emulate();

	return 0;
}