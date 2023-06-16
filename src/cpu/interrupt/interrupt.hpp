#pragma once

#include <iostream>
#include "../8080.hpp"
#include "../instructions.hpp"
#include "../clock/clock.hpp"

/* enumerated abstraction to generate interrupts through
* a simple ISR interface. These are NOT equivalent to the ISR RST opcodes
* you see in opcodes.hpp
*/
enum interrupts
{
	ISR_RST0,
	ISR_RST1,
	ISR_RST2,
	ISR_RST3,
	ISR_RST4,
	ISR_RST5,
	ISR_RST6,
	ISR_RST7
};

class c_interrupthandler
{
public:
	c_interrupthandler()
	{
	}

	void start_render_clock()
	{
		this->render_16hz_clock.start();
	}

	bool check_render_clock()
	{
		auto clock_now = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(clock_now - this->render_16hz_clock.beg).count() > 20)
		{
			this->render_16hz_clock.beg = std::chrono::system_clock::now();
			return true;
		}
		else
		{
			return false;
		}
	}

	void generate_interrupt(interrupts isr, c_8080* ptr)
	{
		if (ptr->enable_interrupts == true)
		{
			switch (isr)
			{
				case ISR_RST0:
				{
					instr::call(ptr->pc, 0x0, ptr->ram.get(), ptr->stackptr);
					break;
				}
				case ISR_RST1:
				{
					instr::call(ptr->pc, 0x8, ptr->ram.get(), ptr->stackptr);
					break;
				}
				case ISR_RST2:
				{
					instr::call(ptr->pc, 0x10, ptr->ram.get(), ptr->stackptr);
					break;
				}
				case ISR_RST3:
				{
					instr::call(ptr->pc, 0x18, ptr->ram.get(), ptr->stackptr);
					break;
				}
				case ISR_RST4:
				{
					instr::call(ptr->pc, 0x20, ptr->ram.get(), ptr->stackptr);
					break;
				}
				case ISR_RST5:
				{
					instr::call(ptr->pc, 0x28, ptr->ram.get(), ptr->stackptr);
					break;
				}
				case ISR_RST6:
				{
					instr::call(ptr->pc, 0x30, ptr->ram.get(), ptr->stackptr);
					break;
				}
				case ISR_RST7:
				{
					instr::call(ptr->pc, 0x37, ptr->ram.get(), ptr->stackptr);
					break;
				}
				default:
				{
					std::printf("ERROR: Invalid ISR!\n");
					break;
				}
			}
		}
	}

	c_clock render_16hz_clock{"renderer"};
private:
};