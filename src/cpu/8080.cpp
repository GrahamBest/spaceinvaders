#include "8080.hpp"
#include "opcodes.hpp"
#include "instructions.hpp"
#include "../cpmbios/cpm.hpp"

void c_8080::emulate()
{
	while (true)
	{
		this->cycle();
		
		if (this->special_registers[PC].val == 5 && 
			this->is_debug_image == true)
			cpm::__bios_operation_0x0005(this);

		this->special_registers[PC].val += 1;
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
			instr::staxb(this->registers[B], this->registers[C], this->registers[A], this->runtime_memory.get());

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
		case DCRC:
		{
			instr::dcrc(this->registers[C], this->flags);

			break;
		}
		case MVICD8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::mvicd8(this->registers[C], byte);

			this->special_registers[PC].val += 1;
			break;
		}
		case RRC:
		{
			instr::rrc(this->registers[A], this->flags);
			break;
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
			instr::staxd(this->registers[D], this->registers[E], this->registers[A], this->runtime_memory.get());

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

			break;
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

			instr::shldadr(byte_1, byte_2, this->runtime_memory.get(), this->registers[H], this->registers[L]);

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
		case DAA:
		{
			instr::daa(this->registers[A], this->flags);

			break;
		}
		case NOP4: { break; }
		case DADH:
		{
			//	instr::dadh(this->registers[H], this->registers[L], this->registers[], this->flags);

			break;
		}
		case LHLDADR:
		{
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];

			instr::lhladr(this->registers[H], this->registers[L], byte_2, byte_1);

			this->special_registers[PC].val += 2;

			break;
		}
		case DCXH:
		{
			instr::dcxh(this->registers[H], this->registers[L]);

			break;
		}
		case INRL:
		{
			instr::inrl(this->registers[L], this->flags);

			break;
		}
		case DCRL:
		{
			instr::dcrl(this->registers[L], this->flags);

			break;
		}
		case MVILD8:
		{
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];

			instr::movfrombyte(this->registers[L], byte_1);

			this->special_registers[PC].val += 1;
			break;
		}
		case CMA:
		{
			instr::cma(this->registers[A]);

			break;
		}
		case NOP5: { break; }
		case LXISPD16:
		{
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];

			instr::lxispd16(this->stackptr, byte_1, byte_2);

			this->special_registers[PC].val += 2;

			break;
		}
		case STAADR:
		{
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];

			instr::staadr(this->runtime_memory.get(), this->registers[A], byte_1, byte_2);

			this->special_registers[PC].val += 2;
			break;
		}
		case INXSP:
		{
			instr::inxsp(this->stackptr);

			break;
		}
		case INRM:
		{
			instr::inrm(this->registers[H], this->registers[L], this->runtime_memory.get(), this->flags);

			break;
		}
		case DCRM:
		{
			instr::dcrm(this->registers[H], this->registers[L], this->runtime_memory.get(), this->flags);

			break;
		}
		case MVIMD8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::mvimd8(this->runtime_memory.get(), this->registers[H], this->registers[L], byte);

			this->special_registers[PC].val += 1;
			break;
		}
		case STC:
		{
			instr::stc(this->flags);

			break;
		}
		case NOP6: { break; }
		case DADSP:
		{
			instr::dadsp(this->stackptr, this->registers[H], this->registers[L], this->flags);

			break;
		}
		case LDAADR:
		{
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
			instr::ldaadr(this->ram.get(), byte_1, byte_2, this->registers[A]);

			this->special_registers[PC].val += 2;
			break;
		}
		case DCXSP:
		{
			instr::dcxsp(this->stackptr);

			break;
		}
		case INRA:
		{
			instr::inra(this->registers[A], this->flags);

			break;
		}
		case DCRA:
		{
			instr::dcra(this->registers[A], this->flags);

			break;
		}
		case MVIAD8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::mviad8(this->registers[A], byte);

			this->special_registers[PC].val += 1;
			break;
		}
		case CMC:
		{
			instr::cmc(this->flags);

			break;
		}
		case MOVBB:
		{
			instr::mov(this->registers[B], this->registers[B]);

			break;
		}
		case MOVBC:
		{
			instr::mov(this->registers[B], this->registers[C]);

			break;
		}
		case MOVBD:
		{
			instr::mov(this->registers[B], this->registers[D]);

			break;
		}
		case MOVBE:
		{
			instr::mov(this->registers[B], this->registers[E]);

			break;
		}
		case MOVBH:
		{
			instr::mov(this->registers[B], this->registers[H]);

			break;
		}
		case MOVBL:
		{
			instr::mov(this->registers[B], this->registers[L]);

			break;
		}
		case MOVBM:
		{
			std::uint16_t hl = this->registers[H].val;
			hl <<= 8;
			hl |= this->registers[L].val;

			std::uint8_t val = this->ram[hl];

			instr::movfrombyte(this->registers[B], val);

			break;
		}
		case MOVBA:
		{
			instr::mov(this->registers[B], this->registers[A]);

			break;
		}
		case MOVCB:
		{
			instr::mov(this->registers[C], this->registers[B]);

			break;
		}
		case MOVCC:
		{
			instr::mov(this->registers[C], this->registers[C]);

			break;
		}
		case MOVCD:
		{
			instr::mov(this->registers[C], this->registers[D]);

			break;
		}
		case MOVCE:
		{
			instr::mov(this->registers[C], this->registers[E]);

			break;
		}
		case MOVCH:
		{
			instr::mov(this->registers[C], this->registers[H]);

			break;
		}
		case MOVCL:
		{
			instr::mov(this->registers[C], this->registers[L]);

			break;
		}
		case MOVCM:
		{
			std::uint16_t hl = this->registers[H].val;
			hl <<= 8;
			hl |= this->registers[L].val;

			std::uint8_t val = this->ram[hl];

			instr::movfrombyte(this->registers[C], val);

			break;
		}
		case MOVCA:
		{
			instr::mov(this->registers[C], this->registers[A]);

			break;
		}
		case MOVDB:
		{
			instr::mov(this->registers[D], this->registers[B]);

			break;
		}
		case MOVDC:
		{
			instr::mov(this->registers[D], this->registers[C]);

			break;
		}
		case MOVDD:
		{
			instr::mov(this->registers[D], this->registers[D]);

			break;
		}
		case MOVDE:
		{
			instr::mov(this->registers[D], this->registers[E]);

			break;
		}
		case MOVDH:
		{
			instr::mov(this->registers[D], this->registers[H]);

			break;
		}
		case MOVDL:
		{
			instr::mov(this->registers[D], this->registers[L]);

			break;
		}
		case MOVDM:
		{
			std::uint16_t hl = this->registers[H].val;
			hl <<= 8;
			hl |= this->registers[L].val;

			std::uint8_t val = this->ram[hl];

			instr::movfrombyte(this->registers[D], val);

			break;
		}
		case MOVDA:
		{
			instr::mov(this->registers[D], this->registers[A]);

			break;
		}
		case MOVEB:
		{
			instr::mov(this->registers[E], this->registers[B]);

			break;
		}
		case MOVEC:
		{
			instr::mov(this->registers[E], this->registers[C]);

			break;
		}
		case MOVED:
		{
			instr::mov(this->registers[E], this->registers[D]);

			break;
		}
		case MOVEE:
		{
			instr::mov(this->registers[E], this->registers[E]);

			break;
		}
		case MOVEH:
		{
			instr::mov(this->registers[E], this->registers[H]);

			break;
		}
		case MOVEL:
		{
			instr::mov(this->registers[E], this->registers[L]);

			break;
		}
		case MOVEM:
		{
			std::uint16_t hl = this->registers[H].val;
			hl <<= 8;
			hl |= this->registers[L].val;

			std::uint8_t val = this->ram[hl];

			instr::movfrombyte(this->registers[E], val);

			break;
		}
		case MOVEA:
		{
			instr::mov(this->registers[E], this->registers[A]);

			break;
		}
		case MOVHB:
		{
			instr::mov(this->registers[H], this->registers[B]);

			break;
		}
		case MOVHC:
		{
			instr::mov(this->registers[H], this->registers[C]);

			break;
		}
		case MOVHD:
		{
			instr::mov(this->registers[H], this->registers[D]);

			break;
		}
		case MOVHE:
		{
			instr::mov(this->registers[H], this->registers[E]);

			break;
		}

		case MOVHH:
		{
			instr::mov(this->registers[H], this->registers[H]);

			break;
		}
		case MOVHL:
		{
			instr::mov(this->registers[H], this->registers[L]);

			break;
		}
		case MOVHM:
		{
			std::uint16_t hl = this->registers[H].val;
			hl <<= 8;
			hl |= this->registers[L].val;

			std::uint8_t val = this->ram[hl];

			instr::movfrombyte(this->registers[H], val);

			break;
		}
		case MOVHA:
		{
			instr::mov(this->registers[H], this->registers[A]);

			break;
		}
		case MOVLB:
		{
			instr::mov(this->registers[L], this->registers[B]);

			break;
		}
		case MOVLC:
		{
			instr::mov(this->registers[L], this->registers[C]);

			break;
		}
		case MOVLD:
		{
			instr::mov(this->registers[L], this->registers[D]);

			break;
		}
		case MOVLE:
		{
			instr::mov(this->registers[L], this->registers[E]);

			break;
		}
		case MOVLH:
		{
			instr::mov(this->registers[L], this->registers[H]);

			break;
		}
		case MOVLL:
		{
			instr::mov(this->registers[L], this->registers[L]);

			break;
		}
		case MOVLM:
		{
			std::uint16_t hl = this->registers[H].val;
			hl <<= 8;
			hl |= this->registers[L].val;

			std::uint8_t val = this->ram[hl];

			instr::movfrombyte(this->registers[L], val);

			break;
		}
		case MOVLA:
		{
			instr::mov(this->registers[L], this->registers[A]);

			break;
		}
		case MOVMB:
		{
			instr::movtomemory(this->runtime_memory.get(), this->registers[H], this->registers[L], this->registers[B]);

			break;
		}
		case MOVMC:
		{
			instr::movtomemory(this->runtime_memory.get(), this->registers[H], this->registers[L], this->registers[C]);

			break;
		}
		case MOVMD:
		{
			instr::movtomemory(this->runtime_memory.get(), this->registers[H], this->registers[L], this->registers[D]);

			break;
		}
		case MOVME:
		{
			instr::movtomemory(this->runtime_memory.get(), this->registers[H], this->registers[L], this->registers[E]);

			break;
		}
		case MOVMH:
		{
			instr::movtomemory(this->runtime_memory.get(), this->registers[H], this->registers[L], this->registers[H]);

			break;
		}
		case MOVML:
		{
			instr::movtomemory(this->runtime_memory.get(), this->registers[H], this->registers[L], this->registers[L]);

			break;
		}
		case HLT:
		{
			std::printf("HALTED\n");

			while (true) { }

			break;
		}
		case MOVMA:
		{
			instr::movtomemory(this->runtime_memory.get(), this->registers[H], this->registers[L], this->registers[A]);

			break;
		}
		case MOVAB:
		{
			instr::mov(this->registers[A], this->registers[B]);

			break;
		}
		case MOVAC:
		{
			instr::mov(this->registers[A], this->registers[C]);

			break;
		}
		case MOVAD:
		{
			instr::mov(this->registers[A], this->registers[D]);

			break;
		}
		case MOVAE:
		{
			instr::mov(this->registers[A], this->registers[E]);

			break;
		}
		case MOVAH:
		{
			instr::mov(this->registers[A], this->registers[H]);

			break;
		}
		case MOVAL:
		{
			instr::mov(this->registers[A], this->registers[L]);

			break;
		}
		case MOVAM:
		{
			std::uint16_t hl = this->registers[H].val;
			hl <<= 8;
			hl |= this->registers[L].val;

			std::uint8_t val = this->ram[hl];

			instr::movfrombyte(this->registers[A], val);
			break;
		}
		case MOVAA:
		{
			instr::mov(this->registers[A], this->registers[A]);

			break;
		}

		case ADDB:
		{
			instr::add_into_a(this->registers[A], this->registers[B], this->flags);

			break;
		}
		case ADDC:
		{
			instr::add_into_a(this->registers[A], this->registers[C], this->flags);

			break;
		}
		case ADDD:
		{
			instr::add_into_a(this->registers[A], this->registers[D], this->flags);

			break;
		}
		case ADDE:
		{
			instr::add_into_a(this->registers[A], this->registers[E], this->flags);

			break;
		}
		case ADDH:
		{
			instr::add_into_a(this->registers[A], this->registers[H], this->flags);

			break;
		}
		case ADDL:
		{
			instr::add_into_a(this->registers[A], this->registers[L], this->flags);

			break;
		}
		case ADDM:
		{
			instr::add_into_a_from_memory(this->registers[A], this->registers[H], this->registers[L], this->ram.get(), this->flags);

			break;
		}
		case ADDA:
		{
			instr::add_into_a(this->registers[A], this->registers[A], this->flags);

			break;
		}
		case ADCB:
		{
			instr::adc(this->registers[A], this->registers[B], this->flags);

			break;
		}
		case ADCC:
		{
			instr::adc(this->registers[A], this->registers[C], this->flags);

			break;
		}
		case ADCD:
		{
			instr::adc(this->registers[A], this->registers[D], this->flags);

			break;
		}
		case ADCE:
		{
			instr::adc(this->registers[A], this->registers[E], this->flags);

			break;
		}
		case ADCH:
		{
			instr::adc(this->registers[A], this->registers[H], this->flags);

			break;
		}
		case ADCL:
		{
			instr::adc(this->registers[A], this->registers[L], this->flags);

			break;
		}
		case ADCM:
		{
			instr::adc_from_memory(this->registers[A], this->registers[H], this->registers[L], this->ram.get(), this->flags);

			break;
		}
		case ADCA:
		{
			instr::adc(this->registers[A], this->registers[A], this->flags);

			break;
		}
		case SUBB:
		{
			instr::sub(this->registers[A], this->registers[B], this->flags);

			break;
		}
		case SUBC:
		{
			instr::sub(this->registers[A], this->registers[C], this->flags);

			break;
		}
		case SUBD:
		{
			instr::sub(this->registers[A], this->registers[D], this->flags);

			break;
		}
		case SUBE:
		{
			instr::sub(this->registers[A], this->registers[E], this->flags);

			break;
		}
		case SUBH:
		{
			instr::sub(this->registers[A], this->registers[H], this->flags);

			break;
		}
		case SUBL:
		{
			instr::sub(this->registers[A], this->registers[L], this->flags);

			break;
		}
		case SUBM:
		{
			instr::sub_from_memory(this->registers[A], this->registers[H], this->registers[L], this->ram.get(), this->flags);

			break;
		}
		case SUBA:
		{
			instr::sub(this->registers[A], this->registers[A], this->flags);

			break;
		}
		case SBBB:
		{
			instr::sbb(this->registers[A], this->registers[B], this->flags);

			break;
		}
		case SBBC:
		{
			instr::sbb(this->registers[A], this->registers[C], this->flags);

			break;
		}
		case SBBD:
		{
			instr::sbb(this->registers[A], this->registers[D], this->flags);

			break;
		}
		case SBBE:
		{
			instr::sbb(this->registers[A], this->registers[E], this->flags);

			break;
		}
		case SBBH:
		{
			instr::sbb(this->registers[A], this->registers[H], this->flags);

			break;
		}
		case SBBL:
		{
			instr::sbb(this->registers[A], this->registers[L], this->flags);

			break;
		}
		case SBBM:
		{
			instr::sbb_from_memory(this->registers[A], this->registers[H], this->registers[L], this->ram.get(), this->flags);

			break;
		}
		case SBBA:
		{
			instr::sbb(this->registers[A], this->registers[A], this->flags);

			break;
		}
		case ANAB:
		{
			instr::and_a(this->registers[A], this->registers[B], this->flags);

			break;
		}
		case ANAC:
		{
			instr::and_a(this->registers[A], this->registers[C], this->flags);

			break;
		}
		case ANAD:
		{
			instr::and_a(this->registers[A], this->registers[D], this->flags);

			break;
		}
		case ANAE:
		{
			instr::and_a(this->registers[A], this->registers[E], this->flags);

			break;
		}
		case ANAH:
		{
			instr::and_a(this->registers[A], this->registers[H], this->flags);

			break;
		}
		case ANAL:
		{
			instr::and_a(this->registers[A], this->registers[L], this->flags);

			break;
		}
		case ANAM:
		{
			instr::and_a_with_memory(this->registers[A], this->registers[H], this->registers[L], this->ram.get(), this->flags);

			break;
		}
		case ANAA:
		{
			instr::and_a(this->registers[A], this->registers[A], this->flags);
			
			break;
		}
		case XRAB:
		{
			instr::xor_a(this->registers[A], this->registers[B], this->flags);

			break;
		}
		case XRAC:
		{
			instr::xor_a(this->registers[A], this->registers[C], this->flags);

			break;
		}
		case XRAD:
		{
			instr::xor_a(this->registers[A], this->registers[D], this->flags);

			break;
		}
		case XRAE:
		{
			instr::xor_a(this->registers[A], this->registers[E], this->flags);

			break;
		}
		case XRAH:
		{
			instr::xor_a(this->registers[A], this->registers[H], this->flags);

			break;
		}
		case XRAL:
		{
			instr::xor_a(this->registers[A], this->registers[L], this->flags);

			break;
		}
		case XRAM:
		{
			instr::xor_a_with_memory(this->registers[A], this->registers[H], this->registers[L], this->ram.get(), this->flags);

			break;
		}
		case XRAA:
		{
			instr::xor_a(this->registers[A], this->registers[A], this->flags);

			break;
		}
		case ORAB:
		{
			instr::or_a(this->registers[A], this->registers[B], this->flags);

			break;
		}
		case ORAC:
		{
			instr::or_a(this->registers[A], this->registers[C], this->flags);

			break;
		}
		case ORAD:
		{
			instr::or_a(this->registers[A], this->registers[D], this->flags);

			break;
		}
		case ORAE:
		{
			instr::or_a(this->registers[A], this->registers[E], this->flags);

			break;
		}
		case ORAH:
		{
			instr::or_a(this->registers[A], this->registers[H], this->flags);

			break;
		}
		case ORAL:
		{
			instr::or_a(this->registers[A], this->registers[L], this->flags);

			break;
		}
		case ORAM:
		{
			instr::or_a_with_memory(this->registers[A], this->registers[H], this->registers[L], this->ram.get(), this->flags);

			break;
		}
		case ORAA:
		{
			instr::or_a(this->registers[A], this->registers[A], this->flags);

			break;
		}
		case CMPB:
		{
			instr::cmp_a(this->registers[A], this->registers[B], this->flags);

			break;
		}
		case CMPC:
		{
			instr::cmp_a(this->registers[A], this->registers[C], this->flags);

			break;
		}
		case CMPD:
		{
			instr::cmp_a(this->registers[A], this->registers[D], this->flags);

			break;
		}
		case CMPE:
		{
			instr::cmp_a(this->registers[A], this->registers[E], this->flags);

			break;
		}
		case CMPH:
		{
			instr::cmp_a(this->registers[A], this->registers[H], this->flags);

			break;
		}
		case CMPL:
		{
			instr::cmp_a(this->registers[A], this->registers[L], this->flags);

			break;
		}
		case CMPM:
		{
			instr::cmp_a_from_memory(this->registers[A], this->registers[H], this->registers[L], this->ram.get(), this->flags);

			break;

		}
		case CMPA:
		{
			instr::cmp_a(this->registers[A], this->registers[A], this->flags);

			break;
		}
		case RNZ:
		{
			if (this->flags[ZERO] != 0)
			{
				instr::ret(this->special_registers[PC], this->stack, this->stackptr);

				this->special_registers[PC].val -= 1;
			}
			break;
		}
		case JNZADR:
		{
			if (this->flags[ZERO] == 1)
			{

				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];
			
				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::jmp(this->special_registers[PC], addr - 0x100);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case JMPADR:
		{
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

			std::uint16_t addr = byte_2;
			addr <<= 8;
			addr |= byte_1;

			instr::jmp(this->special_registers[PC], addr - 0x100);

			this->special_registers[PC].val -= 1;
			break;
		}
		case CNZADR:
		{
			if (flags[ZERO] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::call(this->special_registers[PC], addr - 0x100, this->stack, this->stackptr);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case PUSHB:
		{
			instr::pushb(this->registers[B], this->registers[C], this->stack, this->stackptr);

			break;
		}
		case ADID8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::adid8(this->registers[A], this->flags, byte);

			this->special_registers[PC].val += 1;
			break;
		}
		case RST0:
		{
			instr::call(this->special_registers[PC], 0, this->stack, this->stackptr);

			this->special_registers[PC].val -= 1;
			break;
		}
		case RZ:
		{
			if (this->flags[ZERO] == 0)
			{
				instr::ret(this->special_registers[PC], this->stack, this->stackptr);

				this->special_registers[PC].val -= 1;
			}
			break;
		}
		case RET:
		{
			instr::ret(this->special_registers[PC], this->stack, this->stackptr);

			break;
		}
		case JZADR:
		{
			if (this->flags[ZERO] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::jmp(this->special_registers[PC], addr - 0x100);

				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case NOP7: { break; }
		case CZADR:
		{
			if (flags[ZERO] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::call(this->special_registers[PC], addr - 0x100, this->stack, this->stackptr);

				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break; 
		}
		case CALLADR:
		{
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
			std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

			std::uint16_t addr = byte_2;
			addr <<= 8;
			addr |= byte_1;

			if (addr != 5)
				instr::call(this->special_registers[PC], addr - 0x100, this->stack, this->stackptr);
			else
				instr::call(this->special_registers[PC], addr, this->stack, this->stackptr);

			this->special_registers[PC].val -= 1;
				
			break;
		}
		case POPB:
		{
			instr::popb(this->registers[B], this->registers[C], this->stack, this->stackptr);

			break;
		}
		case ACID8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::acid8(this->registers[A], this->flags, byte);

			this->special_registers[PC].val += 1;
			break;
		}
		case RST1:
		{
			instr::call(this->special_registers[PC], 8, this->stack, this->stackptr);

			this->special_registers[PC].val -= 1;
			break;
		}
		case RNC:
		{
			if (this->flags[CARRY] != 1)
			{
				instr::ret(this->special_registers[PC], this->stack, this->stackptr);

				this->special_registers[PC].val -= 1;
			}
			break;
		}
		case POPD:
		{
			instr::popd(this->registers[D], this->registers[E], this->stack, this->stackptr);

			break;
		}
		case JNCADR:
		{
			if (this->flags[CARRY] != 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::jmp(this->special_registers[PC], addr - 0x100);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case OUTD8:
		{
			/* 
				FINISH OUT LATER
			*/
			this->special_registers[PC].val += 2;
			break;
		}
		case CNCADR:
		{			
			if (flags[CARRY] != 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::call(this->special_registers[PC], addr - 0x100, this->stack, this->stackptr);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case PUSHD:
		{
			instr::pushd(this->registers[D], this->registers[E], this->stack, this->stackptr);

			break;
		}
		case SUID8:
		{
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
			instr::suid8(this->registers[A], byte_1, this->flags);

			this->special_registers[PC].val += 1;
			break;
		}
		case RST2:
		{
			instr::call(this->special_registers[PC], 0x10, this->stack, this->stackptr);

			this->special_registers[PC].val -= 1;
			break;
		}
		case RC:
		{
			if (flags[CARRY] == 1)
			{
				instr::ret(this->special_registers[PC], this->stack, this->stackptr);
			}

			this->special_registers[PC].val -= 1;
			break;
		}
		case NOP8: { break; }
		case JCADR:
		{
			if (this->flags[CARRY] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;
				
				instr::jmp(this->special_registers[PC], addr - 0x100);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case IND8:
		{
			/*
				FINISH IN LATER
			*/
			this->special_registers[PC].val += 2;
			break;
		}
		case CCADR:
		{
			if (this->flags[CARRY] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::call(this->special_registers[PC], addr - 0x100, this->stack, this->stackptr);

				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case NOP9: { break; }
		case SBID8:
		{
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
			instr::sbid8(this->registers[A], byte_1, this->flags);
			this->special_registers[PC].val += 1;

			break;
		}
		case RST3:
		{
			instr::call(this->special_registers[PC], 0x18, this->stack, this->stackptr);

			this->special_registers[PC].val -= 1;
			break;
		}
		case RPO:
		{
			if (this->flags[PARITY] == 0)
			{
				instr::ret(this->special_registers[PC], this->stack, this->stackptr);

				this->special_registers[PC].val -= 1;
			}
			break;
		}
		case POPH:
		{
			instr::poph(this->registers[H], this->registers[L], this->stack, this->stackptr);

			break;
		}
		case JPOADR:
		{
			if (this->flags[PARITY] == 0)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::jmp(this->special_registers[PC], addr - 0x100);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case XTHL:
		{
			instr::xthl(this->registers[H], this->registers[L], this->stack, this->stackptr);

			break;
		}
		case CPOADR:
		{
			if (this->flags[PARITY] == 0)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::call(this->special_registers[PC], addr - 0x100, this->stack, this->stackptr);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case PUSHH:
		{
			instr::pushh(this->registers[H], this->registers[L], this->stack, this->stackptr);

			break;
		}
		case ANID8:
		{
			std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
			instr::anid8(this->registers[A], byte_1, this->flags);

			this->special_registers[PC].val += 1;
			break;
		}
		case RST4:
		{
			instr::call(this->special_registers[PC], 0x20, this->stack, this->stackptr);

			this->special_registers[PC].val -= 1;
			break;
		}
		case RPE:
		{
			if (this->flags[PARITY] == 1)
			{
				instr::ret(this->special_registers[PC], this->stack, this->stackptr);

				this->special_registers[PC].val -= 1;
			}
			break;
		}
		case PCHL:
		{
			instr::pchl(this->registers[H], this->registers[L], this->special_registers[PC]);

			break;
		}
		case JPEADR:
		{
			if (this->flags[PARITY] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::jmp(this->special_registers[PC], addr - 0x100);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case XCHG:
		{
			instr::xchg(this->registers[D], this->registers[E], this->registers[H], this->registers[L]);

			break;
		}
		case CPEADR:
		{
			if (this->flags[PARITY] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::call(this->special_registers[PC], addr - 0x100, this->stack, this->stackptr);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case NOPA: { break; }
		case XRID8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::xrid8(this->registers[A], byte, this->flags);

			this->special_registers[PC].val += 1;
			break;
		}
		case RST5:
		{
			instr::call(this->special_registers[PC], 0x28, this->stack, this->stackptr);

			this->special_registers[PC].val -= 1;
			break;
		}
		case RP:
		{
			if (this->flags[PARITY] == 1)
			{
				instr::ret(this->special_registers[PC], this->stack, this->stackptr);
				special_registers[PC].val -= 1;
			}

			break;
		}
		case POPPSW:
		{
			instr::poppsw(this->registers[A], this->flags, this->stack, this->stackptr);

			break;
		}
		case JPADR:
		{
			if (this->flags[PARITY] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::jmp(this->special_registers[PC], addr - 0x100);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case DI:
		{
			instr::di(this->enable_interrupts);

			break;
		}
		case CPADR:
		{
			if (this->flags[PARITY] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::jmp(this->special_registers[PC], addr - 0x100);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case PUSHPSW:
		{
			instr::pushpsw(this->registers[A], this->stack, this->stackptr, this->flags);

			break; 
		}
		case ORID8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::orid8(this->registers[A], byte, this->flags);

			this->special_registers[PC].val += 1;
			break;
		}
		case RST6:
		{
			instr::call(this->special_registers[PC], 0x30, this->stack, this->stackptr);

			this->special_registers[PC].val -= 1;
			break;
		}
		case RM:
		{
			if (flags[SIGN] == 1)
			{
				instr::ret(this->special_registers[PC], this->stack, this->stackptr);

				this->special_registers[PC].val -= 1;
			}

			break;
		}
		case SPHL:
		{
			instr::sphl(this->stackptr, this->registers[H], this->registers[L]);

			break;
		}
		case JMADR:
		{
			if (flags[SIGN] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::jmp(this->special_registers[PC], addr - 0x100);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break; 
		}
		case EI:
		{
			instr::ei(this->enable_interrupts);

			break;
		}
		case CMADR:
		{
			if (this->flags[SIGN] == 1)
			{
				std::uint8_t byte_1 = this->ram[this->special_registers[PC].val + 1];
				std::uint8_t byte_2 = this->ram[this->special_registers[PC].val + 2];

				std::uint16_t addr = byte_2;
				addr <<= 8;
				addr |= byte_1;

				instr::call(this->special_registers[PC], addr - 0x100, this->stack, this->stackptr);
				this->special_registers[PC].val -= 1;
				break;
			}

			this->special_registers[PC].val += 2;
			break;
		}
		case NOPB: { break; }
		case CPID8:
		{
			std::uint8_t byte = this->ram[this->special_registers[PC].val + 1];
			instr::cpid8(this->registers[A], byte, this->flags);

			this->special_registers[PC].val += 1;
			break;
		}
		case RST7:
		{
			instr::call(this->special_registers[PC], 0x38, this->stack, this->stackptr);

			this->special_registers[PC].val -= 1;
			break;
		}
		default:
		{
			std::printf("FATAL ERROR: UNKNOWN OPCODE 0x%X \n", static_cast<std::uint8_t>(this->ram[this->special_registers[PC].val]));

			break;
		}
	}

	return;
}