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

		if (((b.val & 0x0F) + 1) & 0xF0)
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

		if (((b.val & 0xF0) - 1) & 0x0F)
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
			flag[CARRY] = 0;
			a.val &= 0xFE;
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

		if (((c.val & 0x0F) + 1) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		c.val = val;
	}

	void dcrc(c_register8& c, std::span<std::uint8_t> flags)
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

		if (((c.val & 0xF0) - 1) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		c.val = val;
	}

	void mvicd8(c_register8& c, const std::uint8_t val)
	{
		c.val = val;
	}

	void rrc(c_register8& a, std::span<std::uint8_t> flag)
	{
		std::uint8_t old = a.val;

		a.val >>= 1;

		if (old & 0x01)
		{
			flag[CARRY] = 1;
			a.val |= 0x80;
		}
		else
		{
			flag[CARRY] = 0;
			a.val &= 0x7F;
		}
	}

	void lxidd16(c_register8& d, c_register8& e, std::uint8_t d_value, std::uint8_t e_value)
	{
		d.val = d_value;
		e.val = e_value;
	}

	void staxd(const c_register8& d, const c_register8& e, const c_register8& a, std::uint8_t* memory)
	{

		std::uint16_t memory_address = e.val;

		std::uint16_t high_bits_b = d.val;
		high_bits_b <<= 8;

		memory_address |= high_bits_b;

		memory[memory_address] = a.val;
	}

	void inxd(c_register8& d, c_register8& e)
	{
		std::uint16_t val = e.val;
		
		std::uint16_t high_bits = d.val;
		high_bits <<= 8;

		val |= high_bits;

		val += 1;

		std::uint16_t new_high_bits = val >> 8;
		std::uint8_t new_d = static_cast<std::uint8_t>(new_high_bits);

		std::uint16_t new_low_bits = val & 0xFF;
		std::uint8_t new_e = static_cast<std::uint8_t>(new_low_bits);

		d.val = new_d;
		e.val = new_e;
	}

	void inrd(c_register8& d, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = d.val + 1;

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

		if (((d.val & 0x0F) + 1) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		d.val = val;
	}

	void dcrd(c_register8& d, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = d.val - 1;

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

		if (((d.val & 0xF0) - 1) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		d.val = val;
	}

	void mvidd8(c_register8& d, std::uint8_t byte)
	{
		d.val = byte;
	}

	void ral(c_register8& a, std::span<std::uint8_t> flag)
	{
		std::uint8_t old = a.val;

		a.val <<= 1;
		std::uint8_t old_flag = flag[CARRY];
		
		a.val &= (old_flag | 0xFE);
	
		if (old & 0x80)
		{
			flag[CARRY] = 1;
		}
		else
		{
			flag[CARRY] = 0;
		}
	}

	void dadd(const c_register8& d, const c_register8& e, c_register8& h, c_register8& l, std::span<std::uint8_t> flag)
	{
		std::uint16_t de = e.val;

		std::uint16_t high_bits_d = d.val;
		high_bits_d <<= 8;

		de |= high_bits_d;

		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		hl = hl + de;

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

	void ldaxd(c_register8 a, const c_register8& d, const c_register8& e, const std::uint8_t* memory)
	{
		std::uint16_t memory_address = e.val;

		std::uint16_t high_bits_d = d.val;
		high_bits_d <<= 8;

		memory_address |= high_bits_d;

		a.val = memory[memory_address];
	}

	void dcxd(c_register8& d, c_register8& e)
	{
		std::uint16_t val = e.val;

		std::uint16_t high_bits = d.val;
		high_bits <<= 8;

		val |= high_bits;

		val -= 1;

		std::uint16_t new_high_bits = val >> 8;
		std::uint8_t new_d = static_cast<std::uint8_t>(new_high_bits);

		std::uint16_t new_low_bits = val & 0xFF;
		std::uint8_t new_e = static_cast<std::uint8_t>(new_low_bits);

		d.val = new_d;
		e.val = new_e;
	}

	void inre(c_register8& e, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = e.val + 1;

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

		if (((e.val & 0x0F) + 1) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		e.val = val;
	}

	void dcre(c_register8& e, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = e.val - 1;

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

		if (((e.val & 0xF0) - 1) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		e.val = val;
	}

	void mvied8(c_register8& e, std::uint8_t byte)
	{
		e.val = byte;
	}

	void rar(c_register8& a, std::span<std::uint8_t> flag)
	{
		std::uint8_t old = a.val;

		a.val >>= 1;

		if (old & 0x80)
		{
			a.val |= 0x80;
		}
		else
		{
			a.val &= 0x7F;
		}

		if (old & 0x01)
		{
			flag[CARRY] = 1;
		}
		else
		{
			flag[CARRY] = 0;
		}
	}

	void lxihd16(c_register8& h, c_register8& l, std::uint8_t h_val, std::uint8_t l_val)
	{
		h.val = h_val;
		l.val = l_val;
	}

	void shldadr(const std::uint8_t address_byte_1, const std::uint8_t address_byte_2, std::uint8_t* memory, const c_register8& h, const c_register8& l)
	{
		memory[address_byte_1] = L;
		memory[address_byte_2] = H;
	}

	void inxh(c_register8& h, c_register8& l)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		hl = hl + 1;
		
		std::uint8_t new_low_l = hl & 0xFF;
		l.val = new_low_l;

		std::uint8_t new_low_h = (hl >> 8) & 0xFF;
		h.val = new_low_h;
	}
	
	void inrh(c_register8& h, std::span<std::uint8_t> flags)
	{
		h.val += 1;

		if (h.val == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (h.val & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(h.val))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((h.val & 0x0F) + 1) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}
	}

	void dcrh(c_register8& h, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = h.val - 1;

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

		if (((h.val & 0xF0) - 1) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		h.val = val;
	}

	void mvihd8(c_register8& h, std::uint8_t byte)
	{
		h.val = byte;
	}

	void mov(c_register8& dst, const c_register8& src)
	{
		dst.val = src.val;
	}

	void movfrombyte(c_register8& dst, const std::uint8_t byte)
	{
		dst.val = byte;
	}

	void movtomemory(std::uint8_t* ram, c_register8& h, c_register8& l, c_register8& reg)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		ram[hl] = reg.val;
	}

	void instr::add_into_a(c_register8& a, c_register8& x)
	{

	}
}