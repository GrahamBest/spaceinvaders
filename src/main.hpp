#pragma once

#include <iostream>
#include <raylib.h>

constexpr auto WINDOW_WIDTH = 1200;
constexpr auto WINDOW_HEIGHT = 720;

class c_window
{
public:
	c_window(const std::string& name)
	{
		InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, name.c_str());
		window_name = name;
		SetTargetFPS(60);
	}

	~c_window()
	{
		CloseWindow();
	}

	std::string window_name;
};