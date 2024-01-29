#ifndef AYB_MMU_H
#define AYB_MMU_H

#include "cstdint"

#include "../mbc/IMbc.h"
#include "mem.h"
#include "hw_reg.h"
#include "hw_reg_addr.h"
#include "int_ctrl.h"
#include "../mbc/mbc3.h"

class Mmu {
private:
	IMbc* mbc{};

public:
	explicit Mmu(IMbc* mbcController) : mbc(mbcController) {}
	~Mmu() = default;

	HardwareRegisters hw_reg;
	[[nodiscard]] HardwareRegisters& get_hw_reg() { return hw_reg; }
	Memory mem_util;
	[[nodiscard]] Memory& get_mem_util() { return mem_util; }
	InterruptController int_ctrl;
	[[nodiscard]] InterruptController& get_int_ctrl() { return int_ctrl; }

	uint8_t read_byte(uint16_t addr);
	void write_byte(uint16_t addr, uint8_t val);
	uint16_t read_word(uint16_t addr);
	void write_word(uint16_t addr, uint16_t val);
};

#endif //AYB_MMU_H
