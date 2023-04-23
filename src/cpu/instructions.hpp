#pragma once

#include "registers.hpp"
#include "flags.hpp"
#include <span>

namespace instr
{
	void lxibd16(c_register8& b, c_register8& c, std::uint8_t b_val, std::uint8_t c_val)
	{
		b.val = b_val;
		c.val = c_val;
	}

	void staxb(const c_register8& b, const c_register8& c, const c_register8& a, std::uint8_t* memory)
	{
		std::uint16_t memory_address = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		memory_address |= high_bits_b;
	
		memory[memory_address] = a.val;
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

	void inrb(c_register8& b, std::span<std::uint8_t> flag)
	{
		std::uint16_t val = b.val + 1;

		if (val == 0)
		{
			flag[ZERO] = 1;
		}
		else
		{
			flag[ZERO] = 0;
		}

		if (val & 0x80)
		{
			flag[SIGN] = 1;
		}
		else
		{
			flag[SIGN] = 0;
		}

		if (val > 0xFF)
		{
			flag[CARRY] = 1;
		}
		else
		{
			flag[CARRY] = 0;
		}

		if (((b.val & 0xF) + 1) & 0x10)
		{
			flag[AUXCARRY] = 1;
		}
		else
		{
			flag[AUXCARRY] = 0;
		}

		b.val += 1;
	}

	void dcrb(c_register8& b, std::span<std::uint8_t> flag)
	{
		std::uint16_t val = b.val - 1;

		if (val == 0)
		{
			flag[ZERO] = 1;
		}
		else
		{
			flag[ZERO] = 0;
		}

		if (val & 0x80)
		{
			flag[SIGN] = 1;
		}
		else
		{
			flag[SIGN] = 0;
		}

		if (check_parity8(val))
		{
			flag[PARITY] = 1;
		}
		else
		{
			flag[PARITY] = 0;
		}

		if (((b.val & 0xF0) - 1) & 0xF0)
		{
			flag[AUXCARRY] = 1;
		}
		else
		{
			flag[AUXCARRY] = 0;
		}

		b.val -= 1;
	}

	void mvibd8(c_register8& b, std::uint8_t byte)
	{
		b.val = byte;
	}

	void rlc(c_register8& a, std::span<std::uint8_t> flag)
	{
		std::uint8_t old = a.val;

		a.val <<= 1;

		if (old & 0x80)
		{
			flag[CARRY] = 1;
			a.val |= 1;
		}
		else
		{
			flag[CARRY] = 1;
		}
	}

	void dadb(const c_register8& b, const c_register8& c, c_register8& h, c_register8& l, std::span<std::uint8_t> flag)
	{
		std::uint16_t bc = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		bc |= high_bits_b;

		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		hl = hl + bc;

		if (hl > 0xFFFF)
		{
			flag[CARRY] = 1;
		}
		else
		{
			flag[CARRY] = 0;
		}

		std::uint8_t new_low_l = hl & 0xFF;
		l.val = new_low_l;

		std::uint8_t new_low_h = (hl >> 8) & 0xFF;
		h.val = new_low_h;
	}

	void ldaxb(c_register8 a, const c_register8& b, const c_register8& c, const std::uint8_t* memory)
	{
		std::uint16_t memory_address = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		memory_address |= high_bits_b;

		a.val = memory[memory_address];
	}

	void dcxb(c_register8& b, c_register8& c)
	{
		std::uint16_t bc = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		bc |= high_bits_b;

		bc -= 1;

		std::uint8_t new_low_c = bc & 0xFF;
		c.val = new_low_c;

		std::uint8_t new_low_b = (bc >> 8) & 0xFF;
		b.val = new_low_b;
	}

	void inrc(c_register8& c, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = c.val - 1;

		if (val == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (val & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(val))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((c.val & 0xF) + 1) & 0x10)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		c.val = val;
	}
}