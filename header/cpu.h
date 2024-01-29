#ifndef AYB_CPU_H
#define AYB_CPU_H

#include "cstdint"
#include "stdexcept"

#include "mmu.h"
#include "cpu_reg.h"
#include "cpu_flag.h"
#include "../memory/mem_util.h"
#include "../mbc/IMbc.h"

class Cpu {
private:
	CpuRegisters cpu_reg{};
	CpuFlags     cpu_flag{};
	IMbc* mbc;
	Mmu & mmu;

	int clock_cycles{};
public:
	explicit Cpu(IMbc* mbcController, Mmu& mmu) : mbc(mbcController), mmu(mmu) {
		// Instantiate MMU
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
