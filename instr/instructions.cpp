#include "operator.h"

void Instructions::add_hl(Registers reg) {
	uint16_t hl     = get_reg(HL);
	uint16_t val    = get_reg(reg);
	uint32_t result = hl + val;
	set_flag(N, 0);
	set_flag(HC, (hl & 0x0FFF) + (val & 0x0FFF) > 0x0FFF);
	set_flag(CY, result > 0xFFFF);
	set_reg(HL, result & 0xFFFF);
	cpu.add_clock_cycles(2);
	cpu.get_cpu_reg().add_pc(1);
}
void Instructions::dec_16(Registers reg) {
	set_reg(reg, get_reg(reg) - 1);
	cpu.add_clock_cycles(2);
	cpu.get_cpu_reg().add_pc(1);
}
void Instructions::inc_16(Registers reg) {
	set_reg(reg, get_reg(reg) + 1);
	cpu.add_clock_cycles(2);
	cpu.get_cpu_reg().add_pc(1);
}