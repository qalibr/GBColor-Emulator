#ifndef AYB_MMU_H
#define AYB_MMU_H

#include "cstdint"

#include "IMbc.h"
#include "mem_util.h"
#include "hw_reg.h"
#include "hw_reg_addr.h"

// MBC: IMbc is implemented elsewhere, but it is used here.
//
// MemoryUtil:
// Offers various helpers for accessing memory, for example switching banks.

class Mmu
{
private:
	IMbc* mbc;
	MemoryUtility     mem_util;
	HardwareRegisters hw_reg;

public:
	Mmu();
	~Mmu() = default;
	uint8_t read_byte(uint16_t addr);
	void write_byte(uint16_t addr, uint8_t val);
	uint16_t read_word(uint16_t addr);
	void write_word(uint16_t addr, uint16_t val);
};

#endif //AYB_MMU_H
