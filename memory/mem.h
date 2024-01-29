#ifndef AYB_MEM_H
#define AYB_MEM_H

#include "iostream"
#include "cstdint"
#include "stdexcept"

#include "hw_reg.h"
#include "mem_map.h"

class Memory {
protected:
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

	HardwareRegisters hw_reg;
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
	}
	~Memory() = default;
};

#endif //AYB_MEM_H