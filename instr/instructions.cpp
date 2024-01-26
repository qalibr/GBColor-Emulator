#include "operator.h"

void Instructions::cost(uint8_t cycles, uint8_t size) {
	cpu.add_clock_cycles(cycles);
	cpu.get_cpu_reg().add_pc(size);
}
void Instructions::half_carry_on_add(uint8_t val1, uint8_t val2) {
	set_flag(HC, (((val1 & 0x0F) + (val2 & 0x0F)) & 0x10) > 0x10);
}
void Instructions::half_carry_on_add(uint16_t val1, uint16_t val2) {
	set_flag(HC, (((val1 & 0x00FF) + (val2 & 0x00FF)) & 0x100) > 0x0100);
}
void Instructions::half_carry_on_sub(uint8_t val1, uint8_t val2) {
	set_flag(HC, ((val1 & 0x0F) - (val2 & 0x0F)) < 0);
}
void Instructions::half_carry_on_sub(uint16_t val1, uint16_t val2) {
	set_flag(HC, ((val1 & 0x00FF) - (val2 & 0x00FF)) < 0);
}
void Instructions::nop() {
	cost(1, 4);
}
void Instructions::stop() {
	cpu.get_cpu_flag().set_stop(true);

	if (mem_util.is_speed_switch_pending()) {
		cpu.add_clock_cycles(2052);
		timer.toggle_timer_speed();
		hw_reg.set_div(0);

		/*
		 * If the toggle put us in double speed mote, set bit 7.
		 * If it put us in normal speed mode, clear bit 7.
		 * In both cases we clear bit 0 to indicate that the speed switch is done.
		 */
		hw_reg.set_key1(timer.is_double_speed_enabled() ? 0xFE : 0x7E);
	}

	cost(1, 4);
}
void Instructions::halt() {
	cpu.get_cpu_flag().set_halt(true);
	if (cpu.get_cpu_flag().get_ime()) {
		// TODO: CPU wakes up and handles interrupts.
		// TODO: The ISR needs to check if the CPU is halted and if so, unhalt it.
	}
	else {
		/* Halt bug */
		if (mem_util.is_interrupt_pending()) {
			cpu.get_cpu_flag().set_halt(false); // Wake up from an interrupt.
			cpu.get_cpu_reg().dec_pc(1); // Decrement PC to simulate halt bug.
		}
	}

	cost(1, 4);
}