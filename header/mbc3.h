#ifndef AYB_MBC3_H
#define AYB_MBC3_H

#include "iostream"
#include "cstdint"
#include "vector"
#include "algorithm"
#include "stdexcept"

#include "IMbc.h"

class Mbc3 : public IMbc
{
private:
	static constexpr uint16_t ROM_BANK_SIZE = 0x4000;
	static constexpr uint16_t RAM_BANK_SIZE = 0x2000;
	static constexpr uint16_t RAM_OFFSET    = 0xA000;

	std::vector<uint8_t> rom;
	std::vector<uint8_t> ram;

	int  rom_bank    = 1;
	int  ram_bank    = 0;
	bool ram_enabled = false;

	uint8_t s  = 0;
	uint8_t m  = 0;
	uint8_t h  = 0;
	uint8_t dl = 0;
	uint8_t dh = 0;

public:
	Mbc3(const std::vector<uint8_t>& romData, size_t romSize, size_t ramSize);
	~Mbc3() override = default;

	uint8_t read_rom_high(uint16_t addr) override;
	uint8_t read_rom_low(uint16_t addr) override;
	void write_rom(uint16_t addr, uint8_t val) override;
	uint8_t read_ram(uint16_t addr) override;
	void write_ram(uint16_t addr, uint8_t val) override;
};

#endif //AYB_MBC3_H
