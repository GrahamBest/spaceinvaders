#pragma once

#include <iostream>
#include <chrono>
#include <string>

class c_clock
{
public:
	c_clock(const std::string& reason)
	{
		this->reason = reason;
	}

	void start()
	{
		this->beg = std::chrono::system_clock::now();
	}

	void stop()
	{
		this->end = std::chrono::system_clock::now();
	}

	const std::string& what_for()
	{
		return this->reason;
	}

	std::string reason;
	std::chrono::time_point<std::chrono::system_clock> beg;
	std::chrono::time_point<std::chrono::system_clock> end;
private:
};