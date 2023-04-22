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
		case NOP: { break; } /* do nothing */
		case LXIBD16: 
		{
			std::uint8_t byte_3 = this->ram[this->special_registers[PC].val + 2];
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 1];

			instr::lxibd16(this->registers[B], this->registers[C], byte_3, byte_2);

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
			instr::inrb(this->registers[B], this->registers[FLAGREG]);

			break;
		}
		case DCRB:
		{
			instr::dcrb(this->registers[B], this->registers[FLAGREG]);

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
			instr::rlc(this->registers[A], this->registers[FLAGREG]);

			break;
		}
		case NULL0: { break; }
		case DADB:
		{
			instr::dadb(this->registers[B], this->registers[C], this->registers[H], this->registers[L], this->registers[FLAGREG]);

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