#include "cpu.h"

int Cpu::execute_step() {
	throw std::runtime_error("Execute step not implemented");
}
void Cpu::interrupt_service_routine() {
	throw std::runtime_error("Interrupt service routine not implemented");
}
void Cpu::push(uint16_t val) {
	mmu.write_word(cpu_reg.get_sp(), val);
	cpu_reg.dec_sp(2);
}
uint16_t Cpu::pop() {
	uint16_t sp = mmu.read_word(cpu_reg.get_sp());
	cpu_reg.add_sp(2);
	return sp;
}
void Cpu::add_clock_cycles(int val) {
	clock_cycles += val;
}
uint8_t Cpu::fetch_opcode() {
	uint16_t pc     = cpu_reg.get_pc();
	uint8_t  opcode = mmu.read_byte(pc);
	cpu_reg.add_pc(1);
	return opcode;
}