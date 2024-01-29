#ifndef AYB_MEM_UTIL_H
#define AYB_MEM_UTIL_H

#include "iostream"
#include "cstdint"
#include "stdexcept"

#include "hw_reg.h"
#include "mem_map.h"
#include "mem.h"
#include "bit_util.h"

class MemoryUtility : public Memory {
private:
	HardwareRegisters* hw_reg{};

public:
	MemoryUtility() = default;
	~MemoryUtility() = default;

	bool is_tac_timer_enabled();
	bool is_speed_switch_pending();
	bool is_interrupt_pending();

	int get_tac_timer_freq();

	uint8_t* read_vram_bank(uint16_t addr);
	uint8_t* read_wram_fix(uint16_t addr);
	uint8_t* read_wram_sw(uint16_t addr, int bank);
	uint8_t* read_echo(uint16_t addr);
	uint8_t* read_oam(uint16_t addr);
	uint8_t* read_io(uint16_t addr);
	uint8_t* read_hram(uint16_t addr);

	void switch_vram_bank(uint8_t val);
	void switch_wram_bank(uint8_t val);
	void write_vram_bank(uint16_t addr, uint8_t val);
	void write_wram_bank_fix(uint16_t addr, uint8_t val);
	void write_wram_bank_sw(uint16_t addr, uint8_t val);
	void write_echo(uint16_t addr, uint8_t val);
	void write_oam(uint16_t addr, uint8_t val);
	void write_io(uint16_t addr, uint8_t val);
	void write_hram(uint16_t addr, uint8_t val);
};

#endif //AYB_MEM_UTIL_H
