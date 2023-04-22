#pragma once

#include <cstdint>

enum REGISTERS8
{
	A,
	B,
	C,
	D,
	E,
	H,
	L,
	FLAGREG
};

enum REGISTER16
{
	PC,
	STACKPTR
};

class c_register8
{
public:
	std::uint8_t val;
};

class c_register16
{
public:
	std::uint16_t val;
};