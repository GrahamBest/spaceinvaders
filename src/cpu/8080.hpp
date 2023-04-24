#pragma once

#include "registers.hpp"
#include <memory>
#include <fstream>
#include <stack>
#include <string>
#include <array>

constexpr auto NORMAL_REGISTER_CNT = 8;
constexpr auto SPECIAL_REGISTER_CNT = 2;

class c_8080
{
public:
	c_8080(const std::string& file)
	{
		this->file.open(file.c_str(), std::ios::binary | std::ios::in);

		if (this->file.is_open())
		{
			this->file.seekg(0, std::ios::end);
			this->length = this->file.tellg();
			this->ram = std::make_unique<std::uint8_t[]>(this->length);

			if (this->ram.get() == nullptr)
			{
				std::printf("FATAL ERROR: Couldn't allocate memory!\n");
			}
			
			this->file.seekg(0, std::ios::beg);
			this->file.read(reinterpret_cast<char*>(this->ram.get()), this->length);
		}
		else
		{
			std::printf("FATAL ERROR: Couldn't open %s for emulation!\n", file.c_str());
		}
	}

	void cycle();
	void emulate();

private:
	std::uint32_t length;
	std::stack<std::uint16_t> stack{};
	std::array<c_register8, NORMAL_REGISTER_CNT> registers{}; /* usual general-purpose registers */
	std::array<c_register16, SPECIAL_REGISTER_CNT> special_registers{}; /* stack ptr and pc */
	std::array<std::uint8_t, 5> flags{};
	std::ifstream file{};
	std::unique_ptr<std::uint8_t[]> ram{};
	uint8_t tmp; // 8 Bit temporary register.
	uint8_t act; // 8 Bit temporary accumulator.
	uint8_t ir; // 8 Bit instruction register.
	uint8_t databus[8]; // Bi directional data bus.
};