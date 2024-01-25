#include "mmu.h"

uint8_t Mmu::read_byte(uint16_t addr) {
	switch (addr) {
	case HwRegAddr::VBK:
		return hw_reg.get_vbk();
	case HwRegAddr::SVBK:
		return hw_reg.get_svbk();
	default:
		break;
	}

	switch (addr) {
	case MemoryMap::ROM_FIX_START ... MemoryMap::ROM_FIX_END:
		return mbc->read_rom_low(addr);
	case MemoryMap::ROM_SW_START ... MemoryMap::ROM_SW_END:
		return mbc->read_rom_high(addr);
	case MemoryMap::VRAM_START ... MemoryMap::VRAM_END:
		return *mem_util.read_vram_bank(addr);
	case MemoryMap::EXT_RAM_START ... MemoryMap::EXT_RAM_END:
		return mbc->read_ram(addr);
	case MemoryMap::WRAM_FIX_START ... MemoryMap::WRAM_FIX_END:
		return *mem_util.read_wram_fix(addr);
	case MemoryMap::WRAM_SW_START ... MemoryMap::WRAM_SW_END:
		return *mem_util.read_wram_sw(addr, hw_reg.get_svbk());
	case MemoryMap::ECHO_START ... MemoryMap::ECHO_END:
		return *mem_util.read_echo(addr);
	case MemoryMap::OAM_START ... MemoryMap::OAM_END:
		return *mem_util.read_oam(addr);
	case MemoryMap::UNUSED_START ... MemoryMap::UNUSED_END:
		return 0xFF; // Unused memory returns 0xFF.
	case MemoryMap::IO_START ... MemoryMap::IO_END:
		return *mem_util.read_io(addr);
	case MemoryMap::HRAM_START ... MemoryMap::HRAM_END:
		return *mem_util.read_hram(addr);
	default:
		throw std::out_of_range("Address out of range");
	}
}
void Mmu::write_byte(uint16_t addr, uint8_t val) {
	switch (addr) {
	case HwRegAddr::VBK:
		mem_util.switch_vram_bank(val);
		return;
	case HwRegAddr::SVBK:
		mem_util.switch_wram_bank(val);
		return;
	default:
		break;
	}

	switch (addr) {
	case MemoryMap::ROM_FIX_START ... MemoryMap::ROM_SW_END:
		mbc->write_rom(addr, val);
		break;
	case MemoryMap::VRAM_START ... MemoryMap::VRAM_END:
		mem_util.write_vram_bank(addr, val);
		break;
	case MemoryMap::EXT_RAM_START ... MemoryMap::EXT_RAM_END:
		mbc->write_ram(addr, val);
		break;
	case MemoryMap::WRAM_FIX_START ... MemoryMap::WRAM_FIX_END:
		mem_util.write_wram_bank_fix(addr, val);
		break;
	case MemoryMap::WRAM_SW_START ... MemoryMap::WRAM_SW_END:
		mem_util.write_wram_bank_sw(addr, val);
		break;
	case MemoryMap::ECHO_START ... MemoryMap::ECHO_END:
		mem_util.write_echo(addr, val);
		break;
	case MemoryMap::OAM_START ... MemoryMap::OAM_END:
		mem_util.write_oam(addr, val);
		break;
	case MemoryMap::UNUSED_START ... MemoryMap::UNUSED_END:
		break; // Unused memory is read-only.
	case MemoryMap::IO_START ... MemoryMap::IO_END:
		mem_util.write_io(addr, val);
		break;
	case MemoryMap::HRAM_START ... MemoryMap::HRAM_END:
		mem_util.write_hram(addr, val);
		break;
	default:
		throw std::out_of_range("Address out of range");
	}
}
uint16_t Mmu::read_word(uint16_t addr) {
	return read_byte(addr) | (read_byte(addr + 1) << 8);
}
void Mmu::write_word(uint16_t addr, uint16_t val) {
	write_byte(addr, val & 0xFF);
	write_byte(addr + 1, val >> 8);
}