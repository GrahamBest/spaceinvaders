#pragma once

#include "registers.hpp"
#include "../spaceinvaders/spaceinvaders.hpp"
#include "interrupt/interrupt.hpp"
#include <stack>
#include <memory>
#include <fstream>
#include <stack>
#include <string>
#include <array>
#include <vector>

constexpr auto NORMAL_REGISTER_CNT = 7;
constexpr auto RESERVED = 0x10000;
constexpr auto FLAG_COUNT = 0x05;

class c_8080
{
public:
	c_8080(const std::string& file)
	{
		this->file.open(file.c_str(), std::ios::binary | std::ios::in);
		this->image_name = file;

		this->pc.val = 0x0000;
		this->base = 0x0000;

		if (this->file.is_open())
		{
			this->file.seekg(0, std::ios::end);
			this->length = this->file.tellg();
			this->ram = std::make_unique<std::uint8_t[]>(this->length + RESERVED);

			if (this->ram.get() == nullptr)
			{
				std::printf("FATAL ERROR: Couldn't allocate memory!\n");
			}

			this->file.seekg(0, std::ios::beg);
			this->file.read(reinterpret_cast<char*>(this->ram.get()), this->length);

			this->stackptr = 0xF000;

			this->enable_interrupts = true;
			this->success = true;
		}
		else
		{
			std::printf("FATAL ERROR: Couldn't open %s for emulation!\n", file.c_str());
		}
	}

	void cycle();
	void emulate();


	inline void generate_interrupt(interrupts isr)
	{
		if (this->enable_interrupts == true)
		{
			switch (isr)
			{
				case ISR_RST0:
				{

					break;
				}
				case ISR_RST1:
				{

					break;
				}
				case ISR_RST2:
				{

					break;
				}
				case ISR_RST3:
				{

					break;
				}
				case ISR_RST4:
				{

					break;
				}
				case ISR_RST5:
				{

					break;
				}
				case ISR_RST6:
				{

					break;
				}
				case ISR_RST7:
				{

					break;
				}
				default:
				{
					std::printf("ERROR: Invalid ISR!\n");
					break;
				}
			}
		}
	}

	inline bool is_load_success()
	{
		return this->success;
	}

	bool success{ false };
	bool enable_interrupts;
	std::uint32_t length;
	std::uint16_t stackptr;
	std::array<c_register8, NORMAL_REGISTER_CNT> registers{}; /* usual general-purpose registers */
	c_register16 pc{ 0x0000 }; /* stack ptr and pc */
	std::array<std::uint8_t, FLAG_COUNT> flags{};
	std::ifstream file{};
	std::unique_ptr<std::uint8_t[]> ram{}; /* for rom image */
	std::unique_ptr<std::uint8_t[]> runtime_memory{};
	std::uint16_t base;

	c_spaceinvaders invaders{};

	c_interrupthandler interrupt_handler{};

	std::uint8_t cur_opcode;
	std::string image_name;
	bool is_debug_image = false;
private:
};