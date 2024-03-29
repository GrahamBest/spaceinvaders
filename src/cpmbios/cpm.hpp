#pragma once

/*
*	header only implementation intercept special cp/m bios functions 
*	that some specific romimages use
*/

#include <iostream>
#include "../cpu/8080.hpp"
#include "../cpu/instructions.hpp"

namespace cpm
{
	inline void __bios_operation_0x0005(c_8080* ptr)
	{
		if (ptr->registers[C].val == 2)
		{
			std::printf("%c", ptr->registers[E].val);
		}
		else if (ptr->registers[C].val == 9)
		{
			std::uint16_t addr = ptr->registers[D].val;
			std::uint16_t low = ptr->registers[E].val;
			addr <<= 8;
			addr |= low;

			const char* i = reinterpret_cast<const char*>(&ptr->ram[addr]);

			while (*i != '$')
			{
				std::printf("%c", *i);
				i++;
			}
		}

		instr::ret(ptr->pc, ptr->ram.get(), ptr->stackptr);
	}
}