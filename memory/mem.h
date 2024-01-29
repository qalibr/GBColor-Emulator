#ifndef AYB_MEM_H
#define AYB_MEM_H

#include "iostream"
#include "cstdint"
#include "stdexcept"

#include "hw_reg.h"
#include "mem_map.h"
#include "bit_util.h"

class Memory {
private:
	HardwareRegisters hw_reg{};

	uint8_t VRAM0[8192]{}; // VBK selects bank
	uint8_t* WRAM_BANKS[8]{}; // SVBK selects bank
	uint8_t WRAM0[4096]{};
	uint8_t VRAM1[8192]{};
	uint8_t WRAM1[4096]{};
	uint8_t WRAM2[4096]{};
	uint8_t WRAM3[4096]{};
	uint8_t WRAM4[4096]{};
	uint8_t WRAM5[4096]{};
	uint8_t WRAM6[4096]{};
	uint8_t WRAM7[4096]{};
	uint8_t ECHO[7680]{};
	uint8_t OAM[256]{};
	uint8_t IO[128]{};
	uint8_t HRAM[127]{};

public:
	Memory() {
		WRAM_BANKS[0] = WRAM0;
		WRAM_BANKS[1] = WRAM1;
		WRAM_BANKS[2] = WRAM2;
		WRAM_BANKS[3] = WRAM3;
		WRAM_BANKS[4] = WRAM4;
		WRAM_BANKS[5] = WRAM5;
		WRAM_BANKS[6] = WRAM6;
		WRAM_BANKS[7] = WRAM7;
	};
	~Memory() = default;

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

#endif //AYB_MEM_H
