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
		case ADDC:
		{
			uint8_t sss;

			sss = opcode & 0b00000111;
			tmp = sss; // (SSS)->TMP
			act = this->registers[A].val; // (A)->ACT
			this->registers[A].val = act + tmp; // (ACT)+(TMP)->A

			break;
		}

	}


	this->special_registers[PC].val += 1;
}