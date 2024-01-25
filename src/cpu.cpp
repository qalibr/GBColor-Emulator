#include "cpu.h"

int Cpu::execute_step()
{
	throw std::runtime_error("Execute step not implemented");
}
void Cpu::interrupt_service_routine()
{
	throw std::runtime_error("Interrupt service routine not implemented");
}
void Cpu::push(uint16_t val)
{
	cpu_reg.set_sp(-2);
	mmu.write_word(cpu_reg.get_sp(), val);
}
uint16_t Cpu::pop()
{
	cpu_reg.set_sp(2);
	return mmu.read_word(cpu_reg.get_sp());
}
