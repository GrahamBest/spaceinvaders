#pragma once

enum OPCODES
{
	NOP0 = 0x00,
	LXIBD16 = 0x01,
	STAXB = 0x02,
	INXB = 0x03,
	INRB = 0x04,
	DCRB = 0x05,
	MVIBD8 = 0x06,
	RLC =0x07,
	NULL0 = 0x08,
	DADB = 0x09,
	LDAXB = 0x0A,
	DCXB = 0x0B,
	INRC = 0x0C,
	DCRC = 0x0D,
	MVICD8 = 0x0E,
	RRC = 0x0F,
	NOP1 = 0x10,
	LXIDD16 = 0x11,
	STAXD = 0x12,
	INXD = 0x13,
	INRD = 0x14,
	DCRD = 0x15,
	MVIDD8 = 0x16,
	RAL = 0x17,
	NOP2 = 0x18,
	DADD = 0x19,
	LDAXD = 0x1A,
	DCXD = 0x1B,
	INRE = 0x1C,
	DCRE = 0x1D,
	MVIED8 = 0x1E,
	RAR = 0x1F,
	NOP3 = 0x20,
	LXIHD16 = 0x21,
	SHLDADR = 0x22,
	INXH = 0x23,
	INRH = 0x24,
	DCRH = 0x25,
	MVIHD8 = 0x26,
	DAA = 0x27,
	NOP4 = 0x28,
	DADH = 0x29,

	ADDC = 0x81, // 0b1000 0001
};