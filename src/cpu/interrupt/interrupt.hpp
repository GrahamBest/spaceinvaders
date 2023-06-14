#pragma once

#include <iostream>
#include "../clock/clock.hpp"

class c_interrupthandler
{
public:
	void start_render_clock()
	{
		this->render_16hz_clock.start();
	}

	bool check_render_clock()
	{
		auto clock_now = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(clock_now - this->render_16hz_clock.beg).count() > 100)
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