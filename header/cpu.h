#ifndef AYB_CPU_H
#define AYB_CPU_H

#include "cstdint"
#include "stdexcept"

#include "mmu.h"
#include "cpu_reg.h"
#include "cpu_flag.h"
#include "mem_util.h"

class Cpu
{
private:
	CpuRegisters cpu_reg;
	CpuFlags     cpu_flag;
	Mmu          mmu;
	int          clock_cycles{};

public:
	Cpu() = default;
	~Cpu() = default;

	int execute_step();
	void interrupt_service_routine();
	void push(uint16_t val);
	uint16_t pop();
};

#endif //AYB_CPU_H
