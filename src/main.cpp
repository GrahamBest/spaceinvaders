#include "cpu/8080.hpp"

int main(int argc, char** argv)
{
	c_8080 cpu{ "spaceinvaders.rom" };
	cpu.emulate();

	return 0;
}