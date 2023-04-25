#pragma once

#include <cstdint>
#include <bitset>

enum REGISTERS8
{
	A,
	B,
	C,
	D,
	E,
	H,
	L
};

enum REGISTER16
{
	PC,
	STACKPTR
};

static inline bool check_parity8(const std::bitset<8>& val)
{
	/* check for even number of 1s */
	if (val.count() % 2 == 0)
	{
		return true;
	}
	else
	{
		return false;
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