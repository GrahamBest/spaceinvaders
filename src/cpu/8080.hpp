#pragma once

#include "registers.hpp"
#include <memory>
#include <fstream>
#include <stack>
#include <string>

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
	c_register8 registers[NORMAL_REGISTER_CNT]{}; /* usual general-purpose registers */
	c_register16 special_registers[SPECIAL_REGISTER_CNT]{}; /* stack ptr and pc */
	std::ifstream file{};
	std::unique_ptr<std::uint8_t[]> ram{};
};