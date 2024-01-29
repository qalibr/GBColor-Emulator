#ifndef AYB_MMU_H
#define AYB_MMU_H

#include "cstdint"

#include "../mbc/IMbc.h"
#include "mem_util.h"
#include "hw_reg.h"
#include "hw_reg_addr.h"
#include "int_ctrl.h"
#include "../mbc/mbc3.h"

// MBC: IMbc is implemented elsewhere, but it is used here.
//
// MemoryUtil:
// Offers various helpers for accessing memory, for example switching banks.

class Mmu {
private:
	IMbc* mbc{};

public:
	Mmu(IMbc* mbcController) : mbc(mbcController) {}
	~Mmu() = default;

	HardwareRegisters hw_reg;
	[[nodiscard]] HardwareRegisters& get_hw_reg() { return hw_reg; }
	MemoryUtility mem_util;
	[[nodiscard]] MemoryUtility& get_mem_util() { return mem_util; }
	InterruptController int_ctrl;
	[[nodiscard]] InterruptController& get_int_ctrl() { return int_ctrl; }

	uint8_t read_byte(uint16_t addr);
	void write_byte(uint16_t addr, uint8_t val);
	uint16_t read_word(uint16_t addr);
	void write_word(uint16_t addr, uint16_t val);
};

#endif //AYB_MMU_H
