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
	LHLDADR = 0x2A,
	DCXH = 0x2B,
	INRL = 0x2C,
	DCRL = 0x2D,
	MVILD8 = 0x2E,
	CMA = 0x2F,
	NOP5 = 0x30,
	LXISPD16 = 0x31,
	STAADR,
	INXSP,
	INRM,
	DCRM,
	MVIMD8,
	STC,
	NOP6,
	DADSP,
	LDAADR,
	DCXSP,
	INRA,
	DCRA,
	MVIAD8,
	CMC,
	MOVBB,
	MOVBC,
	MOVBD,
	MOVBE,
	MOVBH,
	MOVBL,
	MOVBM,
	MOVBA,
	MOVCB,
	MOVCC,
	MOVCD,
	MOVCE,
	MOVCH,
	MOVCL,
	MOVCM,
	MOVCA,
	MOVDB,
	MOVDC,
	MOVDD,
	MOVDE,
	MOVDH,
	MOVDL,
	MOVDM = 0x56,
	MOVDA,
	MOVEB,
	MOVEC,
	MOVED,
	MOVEE,
	MOVEH,
	MOVEL,
	MOVEM,
	MOVEA,
	MOVHB,
	MOVHC,
	MOVHD,
	MOVHE,
	MOVHH,
	MOVHL,
	MOVHM,
	MOVHA,
	MOVLB,
	MOVLC,
	MOVLD,
	MOVLE,
	MOVLH,
	MOVLL,
	MOVLM,
	MOVLA,
	MOVMB,
	MOVMC,
	MOVMD,
	MOVME,
	MOVMH,
	MOVML,
	HLT,
	MOVMA = 0x77,
	MOVAB,
	MOVAC,
	MOVAD,
	MOVAE,
	MOVAH,
	MOVAL,
	MOVAM,
	MOVAA,
	ADDB = 0x80,
	ADDC = 0x81,
	ADDD = 0x82,
	ADDE = 0x83,
	ADDH = 0x84,
	ADDL = 0x85,
	ADDM = 0x86,
	ADDA = 0x87,
};