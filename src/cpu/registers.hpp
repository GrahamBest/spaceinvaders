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

static inline std::uint8_t check_parity8(std::uint8_t val)
{
	std::uint8_t x = val ^ (val >> 1);
	x = x ^ (x >> 2);
	x = x ^ (x >> 4);

	if (x & 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

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