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
		if (std::chrono::duration_cast<std::chrono::milliseconds>(clock_now - this->render_16hz_clock.beg).count() > 70)
		{
			this->render_16hz_clock.beg = std::chrono::system_clock::now();
			return true;
		}
		else
		{
			return false;
		}
	}

	c_clock render_16hz_clock{"renderer"};
private:
};