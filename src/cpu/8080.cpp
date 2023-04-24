#include "8080.hpp"
#include "opcodes.hpp"
#include "instructions.hpp"

void c_8080::emulate()
{
	while (true)
	{
		this->cycle();
	}
}

void c_8080::cycle()
{
	std::uint8_t opcode = this->ram[this->special_registers[PC].val];

	switch (opcode)
	{
		case NOP0: { break; } /* do nothing */
		case LXIBD16: 
		{
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];

			instr::lxibd16(this->registers[B], this->registers[C], byte_2, byte_1);

			this->special_registers[PC].val += 2;
			break;
		}
		case STAXB:
		{
			instr::staxb(this->registers[B], this->registers[C], this->registers[A], this->ram.get());

			break;
		}
		case INXB:
		{
			instr::inxb(this->registers[B], this->registers[C]);

			break;
		}
		case INRB:
		{
			instr::inrb(this->registers[B], this->flags);

			break;
		}
		case DCRB:
		{
			instr::dcrb(this->registers[B], this->flags);

			break;
		}
		case MVIBD8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::mvibd8(this->registers[B], byte);

			this->special_registers[PC].val += 1;
			break; 
		}
		case RLC:
		{
			instr::rlc(this->registers[A], this->flags);

			break;
		}
		case NULL0: { break; }
		case DADB:
		{
			instr::dadb(this->registers[B], this->registers[C], this->registers[H], this->registers[L], this->flags);

			break;
		}
		case LDAXB:
		{
			instr::ldaxb(this->registers[A], this->registers[B], this->registers[C], this->ram.get());

			break;
		}
		case DCXB:
		{
			instr::dcxb(this->registers[B], this->registers[C]);

			break;
		}
		case INRC:
		{
			instr::inrc(this->registers[C], this->flags);

			break;
		}
		case MVICD8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::mvicd8(this->registers[C], byte);

			this->special_registers[PC].val += 1;
		}
		case RRC:
		{
			instr::rrc(this->registers[A], this->flags);
		}
		case NOP1: { break;	}
		case LXIDD16:
		{
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];

			instr::lxidd16(this->registers[D], this->registers[E], byte_2, byte_1);

			this->special_registers[PC].val += 2;
			break;
		}
		case STAXD:
		{
			instr::staxd(this->registers[D], this->registers[E], this->registers[A], this->ram.get());

			break;
		}
		case INXD:
		{
			instr::inxd(this->registers[D], this->registers[E]);

			break;
		}
		case INRD:
		{
			instr::inrd(this->registers[D], this->flags);
		}
		case DCRD:
		{
			instr::dcrd(this->registers[D], this->flags);

			break;
		}
		case MVIDD8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::mvidd8(this->registers[D], byte);

			this->special_registers[PC].val += 1;
			break;
		}
		case RAL:
		{
			instr::ral(this->registers[A], this->flags);

			break;
		}
		case NOP2: { break; }
		case DADD:
		{
			instr::dadd(this->registers[D], this->registers[E], this->registers[H], this->registers[L], this->flags);

			break;
		}
		case LDAXD:
		{
			instr::ldaxd(this->registers[A], this->registers[D], this->registers[E], this->ram.get());

			break;
		}
		case DCXD:
		{
			instr::dcxd(this->registers[D], this->registers[E]);
		
			break;
		}
		case INRE:
		{
			instr::inre(this->registers[E], this->flags);

			break; 
		}
		case DCRE:
		{
			instr::dcre(this->registers[E], this->flags);

			break;
		}
		case MVIED8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::mvied8(this->registers[E], byte);

			this->special_registers[PC].val += 1;
			break;
		}
		case RAR:
		{
			instr::rar(this->registers[A], this->flags);

			break;
		}
		case NOP3: { break; }
		case LXIHD16:
		{
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];

			instr::lxihd16(this->registers[H], this->registers[L], byte_2, byte_1);

			this->special_registers[PC].val += 2;

			break;
		}
		case SHLDADR:
		{
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];

			instr::shldadr(byte_1, byte_2, this->ram.get(), this->registers[H], this->registers[L]);

			this->special_registers[PC].val += 2;

			break;
		}
		case INXH:
		{
			instr::inxh(this->registers[H], this->registers[L]);

			break;
		}
		case INRH:
		{
			instr::inrh(this->registers[H], this->flags);

			break;
		}
		case DCRH:
		{
			instr::dcrh(this->registers[H], this->flags);

			break;
		}
		case MVIHD8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::mvihd8(this->registers[H], byte);

			this->special_registers[PC].val += 1;
			break;
		}
		/*
		* IMPLEMENT DAA SOON
		*/
		case DAA:
		{
			break;
		}
		case NOP4: { break; }
		case DADH:
		{
			/* IMPLEMENT NEXT */
			break;
		}
		case ADDC:
		{
			uint8_t sss;

			sss = opcode & 0b00000111;
			tmp = this->registers[sss].val; // (SSS)->TMP
			act = this->registers[A].val; // (A)->ACT
			this->registers[A].val = act + tmp; // (ACT)+(TMP)->A

			break;
		}
		case ADDD:
		{
			uint8_t sss;

			sss = opcode & 0b00000111;
			tmp = this->registers[sss].val; // (SSS)->TMP
			act = this->registers[A].val; // (A)->ACT
			this->registers[A].val = act + tmp; // (ACT)+(TMP)->D

			break;
		}
		case ADDE:
		{
			uint8_t sss;

			sss = opcode & 0b00000111;
			tmp = this->registers[sss].val; // (SSS)->TMP
			act = this->registers[A].val; // (A)->ACT
			this->registers[A].val = act + tmp; // (ACT)+(TMP)->E

			break;
		}
		case ADDH:
		{
			uint8_t sss;

			sss = opcode & 0b00000111;
			tmp = this->registers[sss].val; // (SSS)->TMP
			act = this->registers[A].val; // (A)->ACT
			this->registers[A].val = act + tmp; // (ACT)+(TMP)->H

			break;
		}
		case ADDL:
		{
			uint8_t sss;

			sss = opcode & 0b00000111;
			tmp = this->registers[sss].val; // (SSS)->TMP
			act = this->registers[A].val; // (A)->ACT
			this->registers[A].val = act + tmp; // (ACT)+(TMP)->L

			break;
		}
		case ADDM:
		{
			uint8_t sss;

			sss = opcode & 0b00000111;
			tmp = this->registers[sss].val; // (SSS)->TMP
			act = this->registers[A].val; // (A)->ACT
			this->registers[A].val = act + tmp; // (ACT)+(TMP)->M

			break;
		}
		case ADDA:
		{
			uint8_t sss;

			sss = opcode & 0b00000111;
			tmp = this->registers[sss].val; // (SSS)->TMP
			act = this->registers[A].val; // (A)->ACT
			this->registers[A].val = act + tmp; // (ACT)+(TMP)->A

			break;
		}
	}


	this->special_registers[PC].val += 1;
}