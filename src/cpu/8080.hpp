#pragma once

#include "registers.hpp"
#include <memory>
#include <fstream>
#include <stack>
#include <string>
#include <array>
#include <vector>

constexpr auto NORMAL_REGISTER_CNT = 8;
constexpr auto SPECIAL_REGISTER_CNT = 2;
constexpr auto STACKSIZE = 1024;

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

			this->stack.resize(STACKSIZE);
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
	std::vector<std::uint16_t> stack;
	std::uint16_t stackptr;
	std::array<c_register8, NORMAL_REGISTER_CNT> registers{}; /* usual general-purpose registers */
	std::array<c_register16, SPECIAL_REGISTER_CNT> special_registers{}; /* stack ptr and pc */
	std::array<std::uint8_t, 5> flags{};
	std::ifstream file{};
	std::unique_ptr<std::uint8_t[]> ram{};
};