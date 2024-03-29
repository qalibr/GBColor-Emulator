#include "mem.h"

uint8_t* Memory::read_vram_bank(uint16_t addr) {
	if (hw_reg.get_vbk() == 0) {
		return &VRAM0[addr - MemoryMap::VRAM_END];
	}
	else {
		return &VRAM1[addr - MemoryMap::VRAM_END];
	}
}
uint8_t* Memory::read_wram_fix(uint16_t addr) {
	if (addr <= MemoryMap::WRAM_FIX_START || addr >= MemoryMap::WRAM_FIX_END) {
		throw std::out_of_range("Fixed WRAM bank out of range");
	}

	return &WRAM0[addr - MemoryMap::WRAM_FIX_START];
}
uint8_t* Memory::read_wram_sw(uint16_t addr, int bank) {
	bank = hw_reg.get_svbk();
	if (bank == 0) {
		bank = 1; // 0 is interpreted as 1.
	}

	if (bank < 1 || bank > 7) {
		throw std::out_of_range("Invalid WRAM bank");
	}
	if (addr <= MemoryMap::WRAM_SW_START || addr >= MemoryMap::WRAM_SW_END) {
		throw std::out_of_range("WRAM bank address out of range");
	}

	return &WRAM_BANKS[bank][addr - MemoryMap::WRAM_SW_START];
}
uint8_t* Memory::read_echo(uint16_t addr) {
	return &ECHO[addr - MemoryMap::ECHO_START];
}
uint8_t* Memory::read_oam(uint16_t addr) {
	return &OAM[addr - MemoryMap::OAM_START];
}
uint8_t* Memory::read_io(uint16_t addr) {
	return &IO[addr - MemoryMap::IO_START];
}
uint8_t* Memory::read_hram(uint16_t addr) {
	return &HRAM[addr - MemoryMap::HRAM_START];
}
void Memory::write_vram_bank(uint16_t addr, uint8_t val) {
	if (hw_reg.get_vbk() == 0) {
		VRAM0[addr - MemoryMap::VRAM_START] = val;
	}
	else {
		VRAM1[addr - MemoryMap::VRAM_START] = val;
	}
}
void Memory::write_wram_bank_fix(uint16_t addr, uint8_t val) {
	if (addr <= MemoryMap::WRAM_FIX_START || addr >= MemoryMap::WRAM_FIX_END) {
		throw std::out_of_range("Fixed WRAM bank out of range");
	}

	WRAM_BANKS[0][addr - MemoryMap::WRAM_FIX_START] = val;
}
void Memory::write_wram_bank_sw(uint16_t addr, uint8_t val) {
	int bank = hw_reg.get_svbk();
	if (bank == 0) {
		bank = 1; // 0 is interpreted as 1.
	}

	if (bank < 1 || bank > 7) {
		throw std::out_of_range("Invalid WRAM bank");
	}
	if (addr <= MemoryMap::WRAM_SW_START || addr >= MemoryMap::WRAM_SW_END) {
		throw std::out_of_range("WRAM bank address out of range");
	}

	WRAM_BANKS[bank][addr - MemoryMap::WRAM_SW_START] = val;
}
void Memory::switch_vram_bank(uint8_t val) {
	hw_reg.set_vbk(val);
}
void Memory::switch_wram_bank(uint8_t val) {
	hw_reg.set_svbk(val);
}
void Memory::write_echo(uint16_t addr, uint8_t val) {
	ECHO[addr - MemoryMap::ECHO_START] = val;
}
void Memory::write_oam(uint16_t addr, uint8_t val) {
	OAM[addr - MemoryMap::OAM_START] = val;
}
void Memory::write_io(uint16_t addr, uint8_t val) {
	IO[addr - MemoryMap::IO_START] = val;
}
void Memory::write_hram(uint16_t addr, uint8_t val) {
	HRAM[addr - MemoryMap::HRAM_START] = val;
}
bool Memory::is_tac_timer_enabled() {
	return BitUtil::is_bit_set(hw_reg.get_tac(), 2);
}
bool Memory::is_speed_switch_pending() {
	return BitUtil::is_bit_set(hw_reg.get_tac(), 0);
}
int Memory::get_tac_timer_freq() {
	// Bit 0-1 of TAC register.
	return hw_reg.get_tac() & 0x3;
}
bool Memory::is_interrupt_pending() {
	return hw_reg.get_if() & hw_reg.get_ie();
}