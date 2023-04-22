#pragma once

#include "registers.hpp"
#include "flags.hpp"

namespace instr
{
	void lxibd16(c_register8& b, c_register8& c, std::uint8_t b_val, std::uint8_t c_val)
	{
		b.val = b_val;
		c.val = c_val;
	}

	void staxb(c_register8& b, c_register8& c, c_register8& a, std::uint8_t* ptr_to_memory)
	{
		std::uint16_t memory_address = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		memory_address |= high_bits_b;
	
		ptr_to_memory[memory_address] = a.val;
	}

	void inxb(c_register8& b, c_register8& c)
	{
		std::uint16_t val = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		val |= high_bits_b;

		val += 1;

		b.val = static_cast<std::uint8_t>(val >> 8);
		c.val = static_cast<std::uint8_t>(val & 0xFF);
	}

	void inrb(c_register8& b, c_register8& flag)
	{
		std::uint16_t val = b.val + 1;

		if (val == 0)
		{
			flag.val |= ZERO;
		}
		if (val & 0x80)
		{
			flag.val |= SIGN;
		}
		if (val > 0xFF)
		{
			flag.val |= C;
		}
		if (((b.val & 0xF) + 1) & 0x10)
		{
			flag.val |= AC;
		}

		b.val += 1;
	}

	void dcrb(c_register8& b, c_register8& flag)
	{
		std::uint16_t val = b.val - 1;

		if (val == 0)
		{
			flag.val |= ZERO;
		}
		if (val & 0x80)
		{
			flag.val |= SIGN;
		}
		
		if (((b.val & 0xF) + 1) & 0x10)
		{
			flag.val |= AC;
		}

		b.val -= 1;
	}

	void mvibd8(c_register8& b, std::uint8_t byte)
	{
		b.val = byte;
	}

	void rlc(c_register8& flag)
	{
		std::uint8_t old = flag.val;

		flag.val <<= 1;

		flag.val |= (old >> 7);
	}

	void dadb(const c_register8& b, const c_register8& c, c_register8& h, c_register8& l)
	{
		std::uint16_t val = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		val |= high_bits_b;

	}
}