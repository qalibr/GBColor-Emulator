#ifndef AYB_OP_CODE_MAP_H
#define AYB_OP_CODE_MAP_H

#include "cstdint"

#include "cpu.h"
#include "cpu_reg.h"
#include "cpu_flag.h"
#include "mmu.h"

class OpCodeMap {
private:
	Cpu          cpu;
	CpuRegisters cpu_reg;
	CpuFlags     cpu_flag;
	Mmu          mmu;

public:
	OpCodeMap();
	~OpCodeMap();

	void execute(uint8_t op_code);
};

#endif //AYB_OP_CODE_MAP_H
