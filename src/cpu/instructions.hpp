#pragma once

#include "registers.hpp"
#include "flags.hpp"
#include <span>

bool check_parity8(const std::bitset<8>& val)
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

	/*
	* i can't find any documentation on what HI is supposed to be
	* I've searched the internet and cannot find proper documentation for it
	*/
	void dadh(c_register8& h, c_register8& l, const c_register8& i, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		std::uint32_t hi = i.val;
		std::uint16_t high_bits_hi = h.val;
		high_bits_hi <<= 8;
		hi |= high_bits_hi;

		hl = hl + hi;

		if (hl > 0xFFFF)
		{
			flags[CARRY] = 1;
		}
		else
		{
			flags[CARRY] = 0;
		}

		hl = static_cast<std::uint16_t>(hl);
		hi = static_cast<std::uint16_t>(hi);

		std::uint8_t new_low_l = hl & 0xFF;
		l.val = new_low_l;

		std::uint8_t new_low_h = (hl >> 8) & 0xFF;
		h.val = new_low_h;
	}

	void lhladr(c_register8& h, c_register8& l, const std::uint8_t byte1, const std::uint8_t byte2)
	{
		h.val = byte1;
		l.val = byte2;
	}

	void dcxh(c_register8& h, c_register8& l)
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

	void inrl(c_register8& l, std::span<std::uint8_t> flags)
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

	void dcrl(c_register8& l, std::span<std::uint8_t> flags)
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

	void cma(c_register8& a)
	{
		a.val = ~a.val;
	}

	void lxispd16(std::uint16_t& sp, const std::uint8_t byte_1, const std::uint8_t byte_2)
	{
		std::uint16_t val = byte_1;
		std::uint16_t high_val_bits = static_cast<std::uint16_t>(byte_2 << 8);

		val |= high_val_bits;

		sp = val;
	}

	void staadr(std::uint8_t* ram, const c_register8& a, const std::uint8_t byte_1, const std::uint8_t byte_2)
	{
		std::uint16_t* ram_ptr_16bit = reinterpret_cast<std::uint16_t*>(ram);

		std::uint16_t addr = byte_1;
		std::uint16_t high_val_bits = static_cast<std::uint16_t>(byte_2 << 8);

		addr |= high_val_bits;

		ram_ptr_16bit[addr] = static_cast<std::uint16_t>(a.val);
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

	void add_into_a(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
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

		if (check_parity8(test_carry))
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

	void add_into_a_from_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
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

		if (check_parity8(test_carry))
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

	void inxsp(std::uint16_t& sp)
	{
		sp++;
	}

	void inrm(c_register8& h, c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
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

	void dcrm(c_register8& h, c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
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

	void mvimd8(std::uint8_t* ram, c_register8& h, c_register8& l, std::uint8_t byte)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		ram[hl] = byte;
	}

	void stc(std::span<std::uint8_t> flags)
	{
		flags[CARRY] = 1;
	}

	void dadsp(const std::uint16_t& sp, c_register8& h, c_register8& l, std::span<std::uint8_t> flags)
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

	void ldaadr(std::uint8_t* ram, const std::uint8_t byte_1, const std::uint8_t byte_2, c_register8& a)
	{
		std::uint16_t val = byte_2;
		val <<= 8;
		val |= byte_1;

		a.val = ram[val];
	}

	void dcxsp(std::uint16_t& sp)
	{
		sp -= 1;
	}

	void inra(c_register8& a, std::span<std::uint8_t> flags)
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
	
	void dcra(c_register8& a, std::span<std::uint8_t> flags)
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

	void mviad8(c_register8& a, const std::uint8_t byte)
	{
		a.val = byte;
	}

	void cmc(std::span<std::uint8_t> flags)
	{
		flags[CARRY] = !flags[CARRY];
	}

	void daa(c_register8& a, std::span<std::uint8_t> flags)
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

		if (val == 0)
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

		if (check_parity8(val))
		{
			flags[PARITY] = 1;
		}
		else
		{
			flags[PARITY] = 0;
		}

		a.val = static_cast<std::uint8_t>(val);
	}

	void adc(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint8_t original = a.val;

		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val);
		test_carry += x.val;
		test_carry += flags[CARRY];

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

		if (check_parity8(test_carry))
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
		a.val += flags[CARRY];
	}

	void adc_from_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;

		std::uint8_t original = a.val;
		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val);

		std::uint8_t value = ram[hl];
		test_carry += value;
		test_carry += flags[CARRY];

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

		if (check_parity8(test_carry))
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

		a.val += value;
		a.val += flags[CARRY];
	}

	void sub(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
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

		if (check_parity8(test_carry))
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

	void sub_from_memory(c_register8& a, const c_register8& h, const c_register8& l, const std::uint8_t* ram, std::span<std::uint8_t> flags)
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

		if (check_parity8(test_carry))
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

	void sbb(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
	{
		std::uint8_t original = a.val;

		std::uint16_t test_carry = static_cast<std::uint16_t>(a.val);
		test_carry -= x.val;
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

		if (check_parity8(test_carry))
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
		a.val -= flags[CARRY];
	}

	void sbb_from_memory(c_register8& a, const c_register8& h, const c_register8& l, const std::uint8_t* ram, std::span<std::uint8_t> flags)
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

		if (check_parity8(test_carry))
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

	void and_a(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
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

	void and_a_with_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
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

	void xor_a(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
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

	void xor_a_with_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
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

	void or_a(c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
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

		a.val = val;
	}

	void or_a_with_memory(c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
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

		a.val = val;
	}

	void cmp_a(const c_register8& a, const c_register8& x, std::span<std::uint8_t> flags)
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

	void cmp_a_from_memory(const c_register8& a, const c_register8& h, const c_register8& l, std::uint8_t* ram, std::span<std::uint8_t> flags)
	{
		std::uint32_t hl = l.val;
		std::uint16_t high_bits_h = h.val;
		high_bits_h <<= 8;

		hl |= high_bits_h;
		std::uint8_t value = a.val;
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

	void popb(c_register8& b, c_register8& c, std::span<std::uint16_t> stack, std::uint16_t& stackptr)
	{
		std::uint16_t value = stack[stackptr];
		std::uint8_t b_lo = value & 0xFF;

		b.val = b_lo;

		std::uint8_t c_lo = value >> 8;
		c.val = c_lo;

		stackptr--;
	}

	void jmp(c_register16& pc, const std::uint16_t addr)
	{
		pc.val = addr;
	}

	/* CNZ INLINED
	*  CNZ INLINED
	*  CNZ INLINED
	*/

	void pushb(c_register8& b, c_register8& c, std::span<std::uint16_t> stack, std::uint16_t& stackptr)
	{
		std::uint16_t value = b.val;
		std::uint16_t high = c.val << 8;
	
		value |= high;

		stack[stackptr] = value;

		stackptr++;
	}

	void adid8(c_register8& a, std::span<std::uint8_t> flags, const std::uint8_t byte)
	{
		std::uint8_t val = byte;
		std::int16_t test_carry = static_cast<std::int16_t>(a.val + val);

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

		if (check_parity8(test_carry))
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

	void ret(c_register16& pc, std::span<std::uint16_t> stack, std::uint16_t& stackptr)
	{
		std::uint16_t lovalue = (stack[stackptr]) >> 8;
		std::uint8_t low_function_bytes = static_cast<std::uint8_t>(lovalue);
		std::uint16_t address = low_function_bytes;
		std::uint16_t hivalue = (stack[stackptr] & 0xFF);
		hivalue <<= 8;

		address |= hivalue;

		pc.val = address;

		stackptr--;
	}
	
	/* JZADR INLINED
	*  JZADR INLINED
	*  JZADR INLINED
	*/

	/* CZADR INLINED
	*  CZADR INLINED
	*  CZADR INLINED
	*/

	void call(c_register16& pc, const std::uint16_t addr, std::span<std::uint16_t> stack, std::uint16_t& stackptr)
	{
		/* fix endianness to match the endiannes of the architecture */

		std::uint16_t addr_to_push = pc.val & 0xFF;
		std::uint16_t pc_low = pc.val & 0xFF00;
		
		pc_low >>= 8;
		addr_to_push <<= 8;
		addr_to_push |= pc_low;

		stack[stackptr] = addr_to_push;

		stackptr++;

		pc.val = addr;
	}

	void acid8(c_register8& a, std::span<std::uint8_t> flags, const std::uint8_t byte)
	{
		std::uint8_t val = byte;
		std::int16_t test_carry = static_cast<std::int16_t>(a.val + val + flags[CARRY]);

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

		if (check_parity8(test_carry))
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

	void popd(c_register8& d, c_register8& e, std::span<std::uint16_t> stack, std::uint16_t& stackptr)
	{
		std::uint16_t value = stack[stackptr];
		std::uint8_t d_lo = value & 0xFF;

		d.val = d_lo;

		std::uint8_t e_lo = value >> 8;
		e.val = e_lo;

		stackptr--;
	}

	/* JNC INLINED
	*  JNC INLINED
	*  JNC INLINED
	*/

	/* CNC INLINED
	*  CNC INLINED
	*  CNC INLINED
	*/

	void pushd(c_register8& d, c_register8& e, std::span<std::uint16_t> stack, std::uint16_t& stackptr)
	{
		std::uint16_t value = d.val;
		std::uint16_t high = e.val << 8;

		value |= high;

		stack[stackptr] = value;

		stackptr++;
	}

	void suid8(c_register8& a, std::uint8_t byte, std::span<std::uint8_t> flags)
	{
		std::uint8_t value = a.val - byte;

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
}