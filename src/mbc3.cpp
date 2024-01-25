#include "mbc3.h"

Mbc3::Mbc3(const std::vector<uint8_t>& romData, size_t romSize, size_t ramSize) {
	if (romData.size() > romSize) {
		throw std::invalid_argument("ROM data is larger than specified size");
	}

	rom.resize(romSize);
	std::copy(romData.begin(), romData.end(), rom.begin());

	ram.resize(ramSize);

	std::cout << "MBC3: Init successful" << std::endl;
}
uint8_t Mbc3::read_rom_high(uint16_t addr) {
	return rom[rom_bank * ROM_BANK_SIZE + (addr - ROM_BANK_SIZE)];
}
uint8_t Mbc3::read_rom_low(uint16_t addr) {
	return rom[addr];
}
void Mbc3::write_rom(uint16_t addr, uint8_t val) {
	switch (addr) {
	case 0x0000 ... 0x1FFF:
		ram_enabled = (val & 0x0F) == 0x0A;
		break;
	case 0x2000 ... 0x3FFF:
		rom_bank = (val &= 0x7F) == 0 ? 1 : val;
		break;
	case 0x4000 ... 0x5FFF:
		ram_bank = val & 0x0F;
		break;
	case 0x6000 ... 0x7FFF:
		// TODO: RTC latch
		break;
	default:
		throw std::out_of_range("ROM write out of range");
	}
}
uint8_t Mbc3::read_ram(uint16_t addr) {
	if (!ram_enabled || ram_bank >= 0x04) {
		return 0xFF;
	}

	return ram[ram_bank * RAM_BANK_SIZE + (addr - RAM_OFFSET)];
}
void Mbc3::write_ram(uint16_t addr, uint8_t val) {
	if (!ram_enabled || ram_bank >= 0x04) {
		return;
	}

	ram[ram_bank * RAM_BANK_SIZE + (addr - RAM_OFFSET)] = val;
}