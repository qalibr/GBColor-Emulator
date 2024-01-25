#ifndef AYB_MEM_UTIL_H
#define AYB_MEM_UTIL_H

#include "iostream"
#include "cstdint"
#include "stdexcept"

#include "hw_reg.h"
#include "mem_map.h"
#include "mem.h"

class MemoryUtility : public Memory
{
private:
	HardwareRegisters hw_reg;

public:
	MemoryUtility() = default;
	~MemoryUtility() = default;

	void switch_vram_bank(uint8_t val);
	void switch_wram_bank(uint8_t val);

	uint8_t* read_vram_bank(uint16_t addr);
	uint8_t* read_wram_bank_fix(uint16_t addr);
	uint8_t* read_wram_bank_sw(uint16_t addr, int bank);

	void write_wram_bank_fix(uint16_t addr, uint8_t val);
	void write_wram_bank_sw(uint16_t addr, uint8_t val);
};

#endif //AYB_MEM_UTIL_H
