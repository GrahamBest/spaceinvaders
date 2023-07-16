#pragma once

#include "registers.hpp"
#include "flags.hpp"
#include <span>

inline bool check_parity8(const std::bitset<8>& val)
{
	return val.count() % 2 == 0;
}

static int counter = 0;

namespace instr
{
	inline void lxibd16(c_register8& b, c_register8& c, std::uint8_t b_val, std::uint8_t c_val)
	{
		b.val = b_val;
		c.val = c_val;
	}

	inline void staxb(const c_register8& b, const c_register8& c, const c_register8& a, std::uint8_t* memory, bool debug)
	{
		std::uint16_t memory_address = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		memory_address |= high_bits_b;

		if (debug)
		memory_address -= 0x100;
	
		memory[memory_address] = a.val;
	}

	inline void inxb(c_register8& b, c_register8& c)
	{
		std::uint16_t val = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		val |= high_bits_b;

		val += 1;

		b.val = static_cast<std::uint8_t>(val >> 8);
		c.val = static_cast<std::uint8_t>(val & 0xFF);
	}

	inline void inrb(c_register8& b, std::span<std::uint8_t> flag)
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

	inline void dcrb(c_register8& b, std::array<std::uint8_t, 5>& flag)
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

		if (check_parity8(static_cast<std::uint8_t>(val) & 0xFF))
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

	inline void mvibd8(c_register8& b, std::uint8_t byte)
	{
		b.val = byte;
	}

	inline void rlc(c_register8& a, std::span<std::uint8_t> flag)
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

	inline void dadb(const c_register8& b, const c_register8& c, c_register8& h, c_register8& l, std::span<std::uint8_t> flag)
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

	inline void ldaxb(c_register8& a, const c_register8& b, const c_register8& c, const std::uint8_t* memory, bool debug)
	{
		std::uint16_t memory_address = c.val;

		std::uint16_t high_bits_b = b.val;
		high_bits_b <<= 8;

		memory_address |= high_bits_b;

		if (debug)
		memory_address -= 0x100;

		a.val = memory[memory_address];
	}

	inline void dcxb(c_register8& b, c_register8& c)
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

	inline void inrc(c_register8& c, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = c.val + 1;

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

	inline void dcrc(c_register8& c, std::span<std::uint8_t> flags)
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

	inline void mvicd8(c_register8& c, const std::uint8_t val)
	{
		c.val = val;
	}

	inline void rrc(c_register8& a, std::span<std::uint8_t> flag)
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

	inline void lxidd16(c_register8& d, c_register8& e, std::uint8_t d_value, std::uint8_t e_value)
	{
		d.val = d_value;
		e.val = e_value;
	}

	inline void staxd(const c_register8& d, const c_register8& e, const c_register8& a, std::uint8_t* memory, bool debug)
	{

		std::uint16_t memory_address = e.val;

		std::uint16_t high_bits_b = d.val;
		high_bits_b <<= 8;

		memory_address |= high_bits_b;

		if (debug)
		memory_address -= 0x100;

		memory[memory_address] = a.val;
	}

	inline void inxd(c_register8& d, c_register8& e)
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

	inline void inrd(c_register8& d, std::span<std::uint8_t> flags)
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

	inline void dcrd(c_register8& d, std::span<std::uint8_t> flags)
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

	inline void mvidd8(c_register8& d, std::uint8_t byte)
	{
		d.val = byte;
	}

	inline void ral(c_register8& a, std::span<std::uint8_t> flag)
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

	inline void dadd(const c_register8& d, const c_register8& e, c_register8& h, c_register8& l, std::span<std::uint8_t> flag)
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

	inline void ldaxd(c_register8& a, const c_register8& d, const c_register8& e, const std::uint8_t* memory, bool debug)
	{
		std::uint16_t memory_address = e.val;

		std::uint16_t high_bits_d = d.val;
		high_bits_d <<= 8;

		memory_address |= high_bits_d;

		if (debug)
		memory_address -= 0x100;

		a.val = memory[memory_address];
	}

	inline void dcxd(c_register8& d, c_register8& e)
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

	inline void inre(c_register8& e, std::span<std::uint8_t> flags)
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

	inline void dcre(c_register8& e, std::span<std::uint8_t> flags)
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

	inline void mvied8(c_register8& e, std::uint8_t byte)
	{
		e.val = byte;
	}

	inline void rar(c_register8& a, std::span<std::uint8_t> flag)
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

	inline void lxihd16(c_register8& h, c_register8& l, std::uint8_t h_val, std::uint8_t l_val)
	{
		h.val = h_val;
		l.val = l_val;
	}

	inline void shldadr(const std::uint8_t address_byte_1, const std::uint8_t address_byte_2, std::uint8_t* memory, const c_register8& h, const c_register8& l, bool debug)
	{
		std::uint16_t addr = address_byte_2;
		std::uint16_t low_bits = address_byte_1;

		addr <<= 8;

		addr |= low_bits;

		if (debug)
		addr -= 0x100;

		memory[addr] = l.val;
		memory[addr + 1] = h.val;
	}

	inline void inxh(c_register8& h, c_register8& l)
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
	
	inline void inrh(c_register8& h, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = h.val + 1;

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

		if (((h.val & 0x0F) + 1) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		h.val = val;
	}

	inline void dcrh(c_register8& h, std::span<std::uint8_t> flags)
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

	inline void mvihd8(c_register8& h, std::uint8_t byte)
	{
		h.val = byte;
	}

	/*
	* i can't find any documentation on what HI is supposed to be
	* I've searched the internet and cannot find proper documentation for it
	*/
	inline void dadh(c_register8& h, c_register8& l, std::span<std::uint8_t> flags)
	{

		std::uint16_t hl_adder = l.val;

		std::uint16_t high_bits_h2 = h.val;
		high_bits_h2 <<= 8;

		hl_adder |= high_bits_h2;

		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		hl = hl + hl_adder;

		if (hl > 0xFFFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		std::uint8_t new_low_l = hl & 0xFF;
		l.val = new_low_l;

		std::uint8_t new_low_h = (hl >> 8) & 0xFF;
		h.val = new_low_h;
	}

	inline void lhladr(c_register8& h, c_register8& l, std::uint8_t* ram, const std::uint8_t byte1, const std::uint8_t byte2, bool debug)
	{
		std::uint16_t address = byte1;
		address <<= 8;
		address |= byte2;

		if (debug)
		address -= 0x100;

		h.val = ram[address + 1];
		l.val = ram[address];
	}

	inline void dcxh(c_register8& h, c_register8& l)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		hl -= 1;

		std::uint8_t new_low_l = hl & 0xFF;
		l.val = new_low_l;

		std::uint8_t new_low_h = (hl >> 8) & 0xFF;
		h.val = new_low_h;
	}

	inline void inrl(c_register8& l, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = l.val + 1;

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

		if (((l.val & 0x0F) + 1) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		l.val = val;
	}

	inline void dcrl(c_register8& l, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = l.val - 1;

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

		if (((l.val & 0xF0) - 1) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		l.val = val;
	}

	inline void cma(c_register8& a)
	{
		a.val = ~a.val;
	}

	inline void lxispd16(std::uint16_t& sp, const std::uint8_t byte_1, const std::uint8_t byte_2, bool debug)
	{
		std::uint16_t val = byte_1;
		std::uint16_t high_val_bits = static_cast<std::uint16_t>(byte_2 << 8);

		val |= high_val_bits;

		sp = val;
	}

	inline void staadr(std::uint8_t* ram, const c_register8& a, const std::uint8_t byte_1, const std::uint8_t byte_2, bool debug)
	{

		std::uint16_t addr = byte_1;
		std::uint16_t high_val_bits = static_cast<std::uint16_t>(byte_2 << 8);

		addr |= high_val_bits;

		if (debug)
		addr -= 0x100;

		ram[addr] = a.val;
	}

	inline void mov(c_register8& dst, const c_register8& src)
	{
		dst.val = src.val;
	}

	inline void movfrombyte(c_register8& dst, const std::uint8_t byte)
	{
		dst.val = byte;
	}

	inline void movtomemory(std::uint8_t* ram, c_register8& h, c_register8& l, c_register8& reg)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		ram[hl] = reg.val;
	}

	inline void add_into_a(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint8_t original = a.val;

		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val);
		test_carry += x.val;

		if (test_carry == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (test_carry > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((original & 0x0F) + x.val) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val += x.val;
	}

	inline void add_into_a_from_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		std::uint8_t original = a.val;
		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val);
	
		std::uint8_t value = ram[hl];
		test_carry += value;

		if (test_carry == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (test_carry > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((original & 0x0F) + value) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = static_cast<std::uint8_t>(test_carry);
	}

	inline void inxsp(std::uint16_t& sp)
	{
		sp = sp + 1;
	}

	inline void inrm(c_register8& h, c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		std::uint8_t* val = &ram[hl];

		std::uint8_t inc_val = *val + 1;

		if (inc_val == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (inc_val & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(inc_val))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((*val & 0x0F) + 1) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		*val = inc_val;
	}

	inline void dcrm(c_register8& h, c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		std::uint8_t* val = &ram[hl];

		std::uint8_t dec_val = *val - 1;

		if (dec_val == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (dec_val & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(dec_val))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((*val & 0xF0) - 1) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		*val = dec_val;
	}

	inline void mvimd8(std::uint8_t* ram, c_register8& h, c_register8& l, std::uint8_t byte)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		ram[hl] = byte;
	}

	inline void stc(std::span<std::uint8_t> flags)
	{
		flags[CARRY] = 1;
	}

	inline void dadsp(const std::uint16_t& sp, c_register8& h, c_register8& l, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		hl += sp;

		if (hl > 0xFFFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		std::uint8_t new_low_l = hl & 0xFF;
		l.val = new_low_l;

		std::uint8_t new_low_h = (hl >> 8) & 0xFF;
		h.val = new_low_h;
	}

	inline void ldaadr(std::uint8_t* ram, const std::uint8_t byte_1, const std::uint8_t byte_2, c_register8& a, bool debug)
	{
		std::uint16_t val = byte_2;
		val <<= 8;
		val |= byte_1;

		if (debug)
		val -= 0x100;

		a.val = ram[val];
	}

	inline void dcxsp(std::uint16_t& sp)
	{
		sp = sp - 1;
	}

	inline void inra(c_register8& a, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = a.val + 1;

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

		if (((a.val & 0x0F) + 1) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = val;
	}
	
	inline void dcra(c_register8& a, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = a.val - 1;

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

		if (((a.val & 0xF0) - 1) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = val;
	}

	inline void mviad8(c_register8& a, const std::uint8_t byte)
	{
		a.val = byte;
	}

	inline void cmc(std::span<std::uint8_t> flags)
	{
		flags[CARRY] = !flags[CARRY];
	}

	inline void daa(c_register8& a, std::span<std::uint8_t> flags)
	{
		std::uint16_t val = a.val;

		if ((a.val & 0x0F) > 9 || flags[AUXCARRY])
		{
			val += 0x06;
		}

		if (flags[CARRY] || a.val > 0x99)
		{
			val += 0x60;
		}

		if ((val & 0xFF) == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (val > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (val & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(val) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		a.val = static_cast<std::uint8_t>(val);
	}

	inline void adc(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint8_t original = a.val;

		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val) + x.val + flags[CARRY];

		if (test_carry == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (test_carry > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((original & 0x0F) + x.val) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = static_cast<std::uint8_t>(test_carry);
	}

	inline void adc_from_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		std::uint8_t original = a.val;
		std::uint8_t value = ram[hl];
		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val) + value + flags[CARRY];

		if (test_carry == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (test_carry > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((original & 0x0F) + value) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = static_cast<std::uint8_t>(test_carry);
	}

	inline void sub(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint8_t original = a.val;

		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val);
		test_carry -= x.val;

		if (test_carry == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (test_carry > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((original & 0xF0) - x.val) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val -= x.val;
	}

	inline void sub_from_memory(c_register8& a, const c_register8& h, const c_register8& l, const std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		std::uint8_t original = a.val;
		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val);

		std::uint8_t value = ram[hl];
		test_carry -= value;

		if (test_carry == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (test_carry > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((original & 0xF0) - value) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val -= value;
	}

	inline void sbb(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint8_t original = a.val;

		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val) - x.val - flags[CARRY];

		if (test_carry == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (test_carry > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((original & 0xF0) - x.val) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = static_cast<std::uint8_t>(test_carry);
	}

	inline void sbb_from_memory(c_register8& a, const c_register8& h, const c_register8& l, const std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		std::uint8_t original = a.val;
		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val);

		std::uint8_t value = ram[hl];
		test_carry -= value;
		test_carry -= flags[CARRY];

		if (test_carry == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (test_carry > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((original & 0xF0) - value) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val -= value;
		a.val -= flags[CARRY];
	}

	inline void and_a(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = static_cast<std::uint8_t>(a.val);
		val &= x.val;

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

		a.val = val;
	}

	inline void and_a_with_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;
		std::uint8_t val = a.val;
		val &= ram[hl];

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

		a.val = val;
	}

	inline void xor_a(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = static_cast<std::uint8_t>(a.val);
		val ^= x.val;

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

		a.val = val;
	}

	inline void xor_a_with_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;
		std::uint8_t val = a.val;
		val ^= ram[hl];

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

		a.val = val;
	}

	inline void or_a(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint8_t val = static_cast<std::uint8_t>(a.val);
		val |= x.val;

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

		flags[AUXCARRY] = 0;
		flags[CARRY] = 0;

		a.val = val;
	}

	inline void or_a_with_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags, bool debug)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;
		std::uint8_t val = a.val;

		val |= ram[hl];

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

		flags[AUXCARRY] = 0;
		flags[CARRY] = 0;

		a.val = val;
	}

	inline void cmp_a(const c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint16_t value = static_cast<std::uint16_t>(a.val - x.val);
		std::uint8_t parity = static_cast<std::uint8_t>(value);

		if (value == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}
	
		if (value & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(parity))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (value > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (((a.val & 0xF0) - x.val) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}
	}

	inline void cmp_a_from_memory(const c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;
		std::uint16_t value = a.val;
		std::uint8_t mem = ram[hl];
		value -= mem;

		if (value == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (value & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(value) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (value > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (((a.val & 0xF0) - mem) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}
	}

	/* RNZ INLINED
	*  RNZ INLINED
	*  RNZ INLINED
	*/

	inline void popb(c_register8& b, c_register8& c, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		b.val = stack[stackptr + 1];
		c.val = stack[stackptr];

		stackptr = stackptr + 2;
	}

	inline void jmp(c_register16& pc, const std::uint16_t addr)
	{
		pc.val = addr;
	}

	/* CNZ INLINED
	*  CNZ INLINED
	*  CNZ INLINED
	*/

	inline void pushb(c_register8& b, c_register8& c, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		stackptr = stackptr - 2;

		stack[stackptr] = c.val;
		stack[stackptr + 1] = b.val;
	}

	inline void adid8(c_register8& a, std::span<std::uint8_t> flags, const std::uint8_t byte)
	{
		std::uint8_t val = byte;
		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val + val);
		std::uint8_t zero_chk = static_cast<std::uint8_t>(test_carry);

		if (zero_chk == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (static_cast<std::uint16_t>(test_carry) > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((a.val & 0x0F) + byte) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = static_cast<std::uint8_t>(test_carry);
	}

	/* RST INLINED
	*  RST INLINED
	*  RST INLINED
	*/

	/* RZ INLINED
	*  RZ INLINED
	*  RZ INLINED
	*/

	inline void ret(c_register16& pc, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		std::uint16_t lovalue = stack[stackptr];
		std::uint8_t low_function_bytes = static_cast<std::uint8_t>(lovalue);
		std::uint16_t address = low_function_bytes;
		std::uint16_t hivalue = stack[stackptr + 1];
		hivalue <<= 8;

		address |= hivalue;

		 // std::printf("RETURNED FROM 0x%X, going back to return address 0x%X\n", pc.val, address + 3);
		counter--;
		pc.val = address + 2;
		stackptr = stackptr + 2;

	}
	
	/* JZADR INLINED
	*  JZADR INLINED
	*  JZADR INLINED
	*/

	/* CZADR INLINED
	*  CZADR INLINED
	*  CZADR INLINED
	*/

	inline void call(c_register16& pc, const std::uint16_t addr, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		/* fix endianness to match the endiannes of the architecture */

		stackptr = stackptr - 2;

		std::uint16_t addr_to_push = pc.val & 0xFF;
		std::uint16_t pc_low = pc.val & 0xFF00;
		
		pc_low >>= 8;
		addr_to_push <<= 8;
		addr_to_push |= pc_low;

		// std::printf("RETURN ADDRESS = 0x%X, CALLED FUNCTION 0x%X\n", pc.val, addr);
		stack[stackptr] = pc.val;
		stack[stackptr + 1] = static_cast<std::uint8_t>(pc_low);
		
		pc.val = addr;
	}

	inline void acid8(c_register8& a, std::span<std::uint8_t> flags, const std::uint8_t byte)
	{
		std::uint8_t val = byte;
		std::uint16_t test_carry = 0;

		if (flags[CARRY])
			test_carry = static_cast<std::uint16_t>(a.val + byte + 1);
		else
			test_carry = static_cast<std::uint16_t>(a.val + byte);

		if (test_carry == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (test_carry & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (test_carry > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(test_carry) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (((a.val & 0x0F) + byte) & 0xF0)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = static_cast<std::uint8_t>(test_carry);
	}
	
	/* RST1 INLINED
	*  RST1 INLINED
	*  RST1 INLINED
	*/
	
	/* RNC INLINED
	*  RNC INLINED
	*  RNC INLINED
	*/

	inline void popd(c_register8& d, c_register8& e, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		d.val = stack[stackptr + 1];

		e.val = stack[stackptr];

		stackptr = stackptr + 2;
	}

	/* JNC INLINED
	*  JNC INLINED
	*  JNC INLINED
	*/

	/* CNC INLINED
	*  CNC INLINED
	*  CNC INLINED
	*/

	inline void pushd(c_register8& d, c_register8& e, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		stackptr = stackptr - 2;

		stack[stackptr] = e.val;
		stack[stackptr + 1] = d.val;
	}

	inline void suid8(c_register8& a, std::uint8_t byte, std::span<std::uint8_t> flags)
	{
		std::uint16_t value = static_cast<std::uint16_t>(a.val - byte);

		if (value == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (value & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(value) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (value > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (((a.val & 0xF0) - byte) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = static_cast<std::uint8_t>(value);
	}	
	
	/* RST2 INLINED
	*  RST2 INLINED
	*  RST2 INLINED
	*/

	/* RC INLINED
	*  RC INLINED
	*  RC INLINED
	*/

	/* JCADR INLINED
	*  JCADR INLINED
	*  JCADR INLINED
	*/

	/* IN D8 IMPLEMENT SOON */


	/* CCADR INLINED
	*  CCADR INLINED
	*  CCADR INLINED
	*/

	inline void sbid8(c_register8& a, std::uint8_t byte, std::span<std::uint8_t> flags)
	{
		std::uint16_t value = 0;

		if (flags[CARRY])
			value = static_cast<std::uint16_t>(a.val - byte - 1);
		else
			value = static_cast<std::uint16_t>(a.val - byte);

		if (value == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (value & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(value) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (value > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (((a.val & 0xF0) - byte) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = value & 0xFF;
	}

	/* RST3 INLINED
	*  RST3 INLINED
	*  RST3 INLINED
	*/
	
	/* RPO INLINED
	*  RPO INLINED
	*  RPO INLINED
	*/

	inline void poph(c_register8& h, c_register8& l, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		h.val = stack[stackptr + 1];
		l.val = stack[stackptr];

		stackptr = stackptr + 2;
	}

	/* JPOADR INLINED
	*  JPOADR INLINED
	*  JPOADR INLINED
	*/

	inline void xthl(c_register8& h, c_register8& l, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		std::uint8_t temp_h = h.val;
		std::uint8_t temp_l = l.val;

		std::uint8_t h_lo = stack[stackptr + 1];
		h.val = h_lo;

		std::uint8_t l_lo = stack[stackptr];
		l.val = l_lo;

		stack[stackptr] = temp_l;
		stack[stackptr + 1] = temp_h;
	}

	/* CPOADR INLINED
	*  CPOADR INLINED
	*  CPOADR INLINED
	*/

	inline void pushh(c_register8& h, c_register8& l, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		stackptr = stackptr - 2;


		stack[stackptr] = l.val;
		stack[stackptr + 1] = h.val;
	}

	inline void anid8(c_register8& a, const std::uint8_t byte, std::span<std::uint8_t> flags)
	{
		std::uint8_t value = a.val & byte;

		if (value == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (value & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(value))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (value > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (((a.val & 0xF0) - byte) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = value;
	}

	/* RST4 INLINED
	*  RST4 INLINED
	*  RST4 INLINED
	*/

	/* RPE INLINED
	*  RPE INLINED
	*  RPE INLINED
	*/

	inline void pchl(const c_register8& h, const c_register8& l, c_register16& pc)
	{
		std::uint16_t new_pc = h.val;
		new_pc <<= 8; 

		new_pc |= l.val;

		new_pc -= 0x101;

		pc.val = new_pc;
	}

	/* JPEADR INLINED
	*  JPEADR INLINED
	*  JPEADR INLINED
	*/

	inline void xchg(c_register8& d, c_register8& e, c_register8& h, c_register8& l)
	{
		std::uint8_t h_val = h.val;
		h.val = d.val;
		d.val = h_val;

		std::uint8_t l_val = l.val;
		l.val = e.val;
		e.val = l_val;
	}

	/* CPEADR INLINED
	*  CPEADR INLINED
	*  CPEADR INLINED
	*/

	inline void xrid8(c_register8& a, const std::uint8_t byte, std::span<std::uint8_t> flags)
	{
		std::uint8_t value = a.val ^ byte;

		if (value == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (value & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(value))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (value > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (((a.val & 0xF0) - byte) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = value;
	}

	/* RST5 INLINED
	*  RST5 INLINED
	*  RST5 INLINED
	*/

	/* RP INLINED
	*  RP INLINED
	*  RP INLINED
	*/

	inline void poppsw(c_register8& a, std::span<std::uint8_t> flags, std::uint8_t* stack, std::uint16_t& stackptr)
	{
		std::uint8_t flagreg = static_cast<std::uint8_t>(stack[stackptr]);
	
		if (flagreg & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (flagreg & 0x40)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (flagreg & 0x10)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		if (flagreg & 0x4)
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (flagreg & 0x1)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}
		
		a.val = stack[stackptr + 1];

		stackptr = stackptr + 2;
	}
	
	/* JP INLINED
	*  JP INLINED
	*  JP INLINED
	*/

	inline void di(bool& interrupt_boolean)
	{
		interrupt_boolean = false;
	}

	/* CP INLINED
	*  CP INLINED
	*  CP INLINED
	*/

	/* PUSH PSW IMPLEMENT LATER */
	/* PUSH PSW IMPLEMENT LATER */
	/* PUSH PSW IMPLEMENT LATER */
	inline void pushpsw(const c_register8& a, std::uint8_t* stack, std::uint16_t& stackptr, std::span<std::uint8_t> flags)
	{
		stackptr = stackptr - 2;
		std::uint8_t flagreg = 0x0000;
		
		if (flags[SIGN])
		{
			flagreg |= 0x80;
		}
		
		if (flags[ZERO])
		{
			flagreg |= 0x40;
		}
		
		if (flags[AUXCARRY])
		{
			flagreg |= 0x10;
		}
		 
		if (flags[PARITY])
		{
			flagreg |= 0x4;
		}
		
		if (flags[CARRY])
		{
			flagreg |= 0x1;
		}

		std::uint8_t value = flagreg;
		stack[stackptr] = value;
		stack[stackptr + 1] = a.val;
	}

	inline void orid8(c_register8& a, const std::uint8_t byte, std::span<std::uint8_t> flags)
	{
		std::uint8_t value = a.val | byte;

		if (value == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (value & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(value))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (value > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (((a.val & 0xF0) - byte) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}

		a.val = value;
	}

	/* RST6 INLINED */
	/* RST6 INLINED */
	/* RST6 INLINED */

	/* RM INLINED */
	/* RM INLINED */
	/* RM INLINED */

	inline void sphl(std::uint16_t& stackptr, const c_register8& h, const c_register8& l, bool debug)
	{
		std::uint16_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		stackptr = hl;
	}

	/* JM INLINED */
	/* JM INLINED */
	/* JM INLINED */

	inline void ei(bool& interrupt_boolean)
	{
		interrupt_boolean = true;
	}

	/* CM INLINED */
	/* CM INLINED */
	/* CM INLINED */

	inline void cpid8(const c_register8& a, const std::uint8_t byte, std::span<std::uint8_t> flags)
	{
		std::uint16_t value = a.val - byte;

		if (value == 0)
		{
			flags[ZERO] = 1;
		}
		else
		{
			flags[ZERO] = 0;
		}

		if (value & 0x80)
		{
			flags[SIGN] = 1;
		}
		else
		{
			flags[SIGN] = 0;
		}

		if (check_parity8(static_cast<std::uint8_t>(value) & 0xFF))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		if (value > 0xFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		if (((a.val & 0xF0) - byte) & 0x0F)
		{
			flags[AUXCARRY] = 1;
		}
		else
		{
			flags[AUXCARRY] = 0;
		}
	}

	/* RST7 INLINED */
	/* RST7 INLINED */
	/* RST7 INLINED */
}