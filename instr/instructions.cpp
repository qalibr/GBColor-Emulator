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
	set_flag(STOP, true);

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
	set_flag(HALT, true);
	if (cpu.get_cpu_flag().get_ime()) {
		// TODO: CPU wakes up and handles interrupts.
		// TODO: The ISR needs to check if the CPU is halted and if so, unhalt it.
	}
	else {
		/* Halt bug */
		if (mem_util.is_interrupt_pending()) {
			set_flag(HALT, false); // Wake up from an interrupt.
			cpu.get_cpu_reg().dec_pc(1); // Decrement PC to simulate halt bug.
		}
	}

	cost(1, 4);
}
void OpcodeMap::prefix_cb() {
	uint8_t cb_op_code = fetch_byte();
	if (cb_instructions[cb_op_code]) {
		cb_instructions[cb_op_code]();
	}
	else {
		throw std::runtime_error("Invalid CB-prefixed opcode");
	}
	cost(1, 4);
}
void Instructions::di() {
	set_flag(IME, false);
	cost(1, 4);
}
void Instructions::ei() {
	cpu.get_cpu_flag().set_enable_interrupts_pending(true);
	cost(1, 4);
}
void Instructions::rlca() {
	uint8_t new_bit0 = (get_reg(A) & 0x80) >> 7;
	uint8_t res      = (get_reg(A) << 1) | new_bit0;
	set_flag(Z, false);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, new_bit0 == 1);
	set_reg(A, res);
	cost(1, 4);
}
void Instructions::rla() {
	uint8_t old_bit7 = (get_reg(A) & 0x80) >> 7;
	uint8_t res      = (get_reg(A) << 1) | get_flag(CY);
	set_flag(Z, false);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, old_bit7 == 1);
	set_reg(A, res);
	cost(1, 4);
}
void Instructions::rrca() {
	uint8_t new_bit7 = (get_reg(A) & 0x01) << 7;
	uint8_t res      = (get_reg(A) >> 1) | new_bit7;
	set_flag(Z, false);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, new_bit7 == 1);
	set_reg(A, res);
	cost(1, 4);
}
void Instructions::rra() {
	uint8_t old_bit0 = (get_reg(A) & 0x01) << 7;
	uint8_t res      = (get_reg(A) >> 1) | (get_flag(CY) << 7);
	set_flag(Z, false);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, old_bit0 == 1);
	set_reg(A, res);
	cost(1, 4);
}
void Instructions::jp_hl() {
	set_reg(PC, get_reg(HL));
	cost(1, 4);
}
void Instructions::reti() {
	set_reg(PC, cpu.pop());
	set_flag(IME, true);
	cost(1, 4);
}