#ifndef AYB_MMU_H
#define AYB_MMU_H

#include "cstdint"

#include "../mbc/IMbc.h"
#include "../memory/mem_util.h"
#include "../memory/hw_reg.h"
#include "../memory/hw_reg_addr.h"
#include "../mbc/mbc3.h"

// MBC: IMbc is implemented elsewhere, but it is used here.
//
// MemoryUtil:
// Offers various helpers for accessing memory, for example switching banks.

class Mmu {
private:
	IMbc* mbc{};
	MemoryUtility mem_util;
	HardwareRegisters& hw_reg;

public:
	explicit Mmu(HardwareRegisters& hw_registers, IMbc* mbcController)
			: hw_reg(hw_registers), mbc(mbcController), mem_util(hw_registers) {}
	~Mmu() = default;

	[[nodiscard]] HardwareRegisters& get_hw_reg() { return hw_reg; }

	uint8_t read_byte(uint16_t addr);
	void write_byte(uint16_t addr, uint8_t val);
	uint16_t read_word(uint16_t addr);
	void write_word(uint16_t addr, uint16_t val);
};

#endif //AYB_MMU_H
