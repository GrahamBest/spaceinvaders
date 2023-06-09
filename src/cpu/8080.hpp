#pragma once

#include "registers.hpp"
#include <memory>
#include <fstream>
#include <stack>
#include <string>
#include <array>
#include <vector>

constexpr auto NORMAL_REGISTER_CNT = 7;
constexpr auto SPECIAL_REGISTER_CNT = 2;
constexpr auto STACKSIZE = 0x10000;

class c_8080
{
public:
	c_8080(const std::string& file)
	{
		this->file.open(file.c_str(), std::ios::binary | std::ios::in);
		this->image_name = file;

		if (this->file.is_open())
		{
			this->file.seekg(0, std::ios::end);
			this->length = this->file.tellg();
			this->ram = std::make_unique<std::uint8_t[]>(this->length + 0x100);

			this->runtime_memory = std::make_unique<std::uint8_t[]>(0x10000);

			if (this->ram.get() == nullptr)
			{
				std::printf("FATAL ERROR: Couldn't allocate memory!\n");
			}
			
			this->file.seekg(0, std::ios::beg);
			this->file.read(reinterpret_cast<char*>(this->ram.get()), this->length);

			this->stack.resize(STACKSIZE);

			/* some intel 8080 rom images for CP/M use a different base
			* 0x100 is the base address of the rom image for debug images 
			* we set the base to this so we can fix calculations to fix our
			* 0x0000 base.
			*/

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

	inline bool is_load_success()
	{
		return this->success;
	}

	bool success{ false };
	bool enable_interrupts;
	std::uint32_t length;
	std::vector<std::uint16_t> stack;
	std::uint16_t stackptr;
	std::array<c_register8, NORMAL_REGISTER_CNT> registers{}; /* usual general-purpose registers */
	std::array<c_register16, SPECIAL_REGISTER_CNT> special_registers{}; /* stack ptr and pc */
	std::array<std::uint8_t, 5> flags{};
	std::ifstream file{};
	std::unique_ptr<std::uint8_t[]> ram{}; /* for rom image */
	std::unique_ptr<std::uint8_t[]> runtime_memory{};
	std::uint16_t base;

	std::string image_name;
	bool is_debug_image = false;
private:
};