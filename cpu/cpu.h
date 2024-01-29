#ifndef AYB_CPU_H
#define AYB_CPU_H

#include "cstdint"
#include "stdexcept"

#include "cpu_reg.h"
#include "cpu_flag.h"
// Must not depend on any sub-files, for /memory only mmu
#include "../mbc/IMbc.h"
#include "../memory/mmu.h"

class Cpu {
private:
	CpuRegisters cpu_reg{};
	CpuFlags     cpu_flag{};
	IMbc* mbc;
	Mmu  mmu;

	int clock_cycles{};
public:
	explicit Cpu(IMbc* mbcController, Mmu mmu) : mbc(mbcController), mmu(mmu) {
		// Instantiate MMU with initialized mbc. This is passed on to operator.h
		mmu = Mmu(mbc);
	}
	~Cpu() = default;

	[[nodiscard]] CpuRegisters& get_cpu_reg() { return cpu_reg; }
	[[nodiscard]] CpuFlags& get_cpu_flag() { return cpu_flag; }

	uint8_t fetch_opcode();
	int execute_step();
	void interrupt_service_routine();
	void push(uint16_t val);
	uint16_t pop();

	void add_clock_cycles(int val);
};

#endif //AYB_CPU_H
