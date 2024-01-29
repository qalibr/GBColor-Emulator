#include "operator.h"

/* Instruction reference
 * https://rgbds.gbdev.io/docs/v0.6.1/gbz80.7#INSTRUCTION_OVERVIEW
 *
 * DAA
 * https://forums.nesdev.org/viewtopic.php?p=196282&sid=c09957170f76c60f51bc270df9c829e5#p196282
 */
void Instructions::cost(uint8_t size, uint8_t cycles) {
	cpu.get_cpu_reg().add_pc(size);
	cpu.add_clock_cycles(cycles);
}
/* Half carry implementation source:
 * https://stackoverflow.com/a/57822729
 */
void Instructions::half_carry_on_add(uint8_t val1, uint8_t val2) {
	set_flag(HC, (((val1 & 0x0F) + (val2 & 0x0F)) & 0x10) >= 0x10);
}
void Instructions::half_carry_on_add(uint16_t val1, uint16_t val2) {
	set_flag(HC, (((val1 & 0x00FF) + (val2 & 0x00FF)) & 0x100) >= 0x0100);
}
void Instructions::half_carry_on_sub(uint8_t val1, uint8_t val2) {
	set_flag(HC, ((val1 & 0x0F) - (val2 & 0x0F)) < 0);
}
void Instructions::half_carry_on_sub(uint16_t val1, uint16_t val2) {
	set_flag(HC, ((val1 & 0x00FF) - (val2 & 0x00FF)) < 0);
}
void Instructions::unused() {
	// TODO:
}
void Instructions::nop() {
	cost(1, 4);
}
void Instructions::stop() {
	set_flag(STOP, true);

	if (mmu.mem_util.is_speed_switch_pending()) {
		cpu.add_clock_cycles(2052);
		timer.toggle_timer_speed();
		mmu.hw_reg.set_div(0);

		/*
		 * If the toggle put us in double speed mote, set bit 7.
		 * If it put us in normal speed mode, clear bit 7.
		 * In both cases we clear bit 0 to indicate that the speed switch is done.
		 */
		mmu.hw_reg.set_key1(timer.is_double_speed_enabled() ? 0xFE : 0x7E);
	}

	cost(1, 4);
}
void Instructions::halt() {
	set_flag(HALT, true);
	if (cpu.get_cpu_flag().get_ime()) {
		// TODO: CPU wakes up and handles interrupts.
		// TODO: The ISR needs to check if the CPU is halted and if so, unhalt it.
	} else {
		/* Halt bug */
		if (mmu.mem_util.is_interrupt_pending()) {
			set_flag(HALT, false); // Wake up from an interrupt.
			cpu.get_cpu_reg().dec_pc(1); // Decrement PC to simulate halt bug.
		}
	}

	cost(1, 4);
}
void OpcodeMap::prefix_cb() {
	/*
	 * https://www.reddit.com/r/EmuDev/comments/7qf352/comment/dsp9jtx/?utm_source=share&utm_medium=web2x&context=3
	 * This prefix_cb() requires atomicity.
	 * TODO: Consider not treating this as an instruction.
	 */
	uint8_t cb_op_code = fetch_byte();
	if (cb_instructions[cb_op_code]) {
		cb_instructions[cb_op_code]();
	} else {
		throw std::runtime_error("error, prefix_cb");
	}
	cost(0, 4);
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
	cost(1, 16);
}
void Instructions::jr_cc_e8(Cc cc) {
	int8_t e8 = fetch_signed_byte();
	bool   jump;

	switch (cc) {
	case nz:
		jump = !get_flag(Z);
		break;
	case z:
		jump = get_flag(Z);
		break;
	case nc:
		jump = !get_flag(CY);
		break;
	case c:
		jump = get_flag(CY);
		break;
	default:
		throw std::runtime_error("error, jr_cc_e8");
	}

	if (jump) {
		set_reg(PC, get_reg(PC) + e8);
		cost(2, 12);
	} else {
		cost(2, 8);
	}
}
void Instructions::jr_e8() {
	int8_t e8 = fetch_signed_byte();
	set_reg(PC, get_reg(PC) + e8);
	cost(2, 12);
}
void Instructions::jp_cc_a16(Cc cc) {
	uint16_t a16 = fetch_address();
	bool     jump;

	switch (cc) {
	case nz:
		jump = !get_flag(Z);
		break;
	case z:
		jump = get_flag(Z);
		break;
	case nc:
		jump = !get_flag(CY);
		break;
	case c:
		jump = get_flag(CY);
		break;
	default:
		throw std::runtime_error("error, jp_cc_a16");
	}

	if (jump) {
		set_reg(PC, a16);
		cost(3, 16);
	} else {
		cost(3, 12);
	}
}
void Instructions::jp_a16() {
	uint16_t a16 = fetch_address();
	set_reg(PC, a16);
	cost(3, 16);
}
void Instructions::call_cc_a16(Cc cc) {
	uint16_t a16 = fetch_address();
	bool     call;

	switch (cc) {
	case nz:
		call = !get_flag(Z);
		break;
	case z:
		call = get_flag(Z);
		break;
	case nc:
		call = !get_flag(CY);
		break;
	case c:
		call = get_flag(CY);
		break;
	default:
		throw std::runtime_error("error, call_cc_a16");
	}

	if (call) {
		cpu.push(get_reg(PC) + 3);
		set_reg(PC, a16);
		cost(3, 24);
	} else {
		cost(3, 12);
	}
}
void Instructions::call_a16() {
	uint16_t a16 = fetch_address();
	cpu.push(get_reg(PC) + 3);
	set_reg(PC, a16);
	cost(3, 24);
}
void Instructions::ret_cc(Cc cc) {
	bool ret;

	switch (cc) {
	case nz:
		ret = !get_flag(Z);
		break;
	case z:
		ret = get_flag(Z);
		break;
	case nc:
		ret = !get_flag(CY);
		break;
	case c:
		ret = get_flag(CY);
		break;
	default:
		throw std::runtime_error("error, ret_cc");
	}

	if (ret) {
		set_reg(PC, cpu.pop());
		cost(1, 20);
	} else {
		cost(1, 8);
	}
}
void Instructions::ret() {
	set_reg(PC, cpu.pop());
	cost(1, 16);
}
void Instructions::rst(Rst rst) {
	cpu.push(get_reg(PC));
	set_reg(PC, rst);
	cost(1, 16);
}
void Instructions::ld_r16_n16(Reg reg) {
	try {
		debug_pc_start(); // TODO: Remove
		uint16_t n16 = fetch_address();
		set_reg(reg, n16);
		cost(3, 12);
		debug_pc_end(get_reg(PC), 3);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
void Instructions::ld_a16_sp() {
	uint16_t n16 = fetch_address();
	write_byte(n16, get_reg(SP) & 0xFF);
	write_byte(n16, get_reg(SP) >> 8);
	cost(5, 20);
}
void Instructions::ld_pop_r16(Reg reg) {
	switch (reg) {
	case BC:
		set_reg(BC, cpu.pop());
		break;
	case DE:
		set_reg(DE, cpu.pop());
		break;
	case HL:
		set_reg(HL, cpu.pop());
		break;
	case AF:
		set_reg(AF, cpu.pop() & 0xFFF0); // Lower 4 bits are always 0.
		break;
	default:
		throw std::runtime_error("error, ld_pop_r16");
	}

	cost(1, 12);
}
void Instructions::ld_push_r16(Reg reg) {
	switch (reg) {
	case BC:
		cpu.push(get_reg(BC));
		break;
	case DE:
		cpu.push(get_reg(DE));
		break;
	case HL:
		cpu.push(get_reg(HL));
		break;
	case AF:
		cpu.push(get_reg(AF) & 0xFFF0);
		break;
	default:
		throw std::runtime_error("error, ld_push_r16");
	}

	cost(1, 16);
}
void Instructions::ld_hl_sp_e8() {
	int8_t   e8  = fetch_signed_byte();
	uint16_t sp  = get_reg(SP);
	uint16_t res = sp + static_cast<uint16_t>(e8);
	set_flag(Z, false);
	set_flag(N, false);
	half_carry_on_add(sp, e8);
	set_flag(CY, ((sp & 0xFF) + (e8 & 0xFF)) > 0xFF);
	set_reg(HL, res);
	cost(2, 12);
}
void Instructions::ld_sp_hl() {
	set_reg(SP, get_reg(HL));
	cost(1, 8);
}
void Instructions::ld_bc_a() {
	uint16_t addr = get_reg(BC);
	uint8_t  val  = get_reg(A);
	write_byte(addr, val);
	cost(1, 8);
}
void Instructions::ld_de_a() {
	uint16_t addr = get_reg(DE);
	uint8_t  val  = get_reg(A);
	write_byte(addr, val);
	cost(1, 8);
}
void Instructions::ld_a_bc() {
	uint16_t addr = get_reg(BC);
	uint8_t  val  = read_byte(addr);
	set_reg(A, val);
	cost(1, 8);
}
void Instructions::ld_a_de() {
	uint16_t addr = get_reg(DE);
	uint8_t  val  = read_byte(addr);
	set_reg(A, val);
	cost(1, 8);
}
void Instructions::ld_c_a() {
	uint16_t addr = get_reg(C) + 0xFF00;
	uint8_t  val  = get_reg(A);
	write_byte(addr, val);
	cost(1, 8);
}
void Instructions::ld_a_c() {
	uint16_t addr = get_reg(C) + 0xFF00;
	uint8_t  val  = read_byte(addr);
	set_reg(A, val);
	cost(1, 8);
}
void Instructions::ld_r8_hl(Reg reg) {
	try {
		debug_pc_start(); // TODO: Remove
		uint16_t val = read_byte(get_reg(HL));
		set_reg(reg, val);
		cost(1, 8);
		debug_pc_end(get_reg(PC), 1);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
void Instructions::ld_hl_r8(Reg reg) {
	uint16_t addr = get_reg(HL);
	uint8_t  val  = get_reg(reg);
	write_byte(addr, val);
	cost(1, 8);
}
void Instructions::ld_hli_a() {
	uint16_t addr = get_reg(HL);
	uint8_t  val  = get_reg(A);
	write_byte(addr, val);
	set_reg(HL, addr + 1);
	cost(1, 8);
}
void Instructions::ld_hld_a() {
	uint16_t addr = get_reg(HL);
	uint8_t  val  = get_reg(A);
	write_byte(addr, val);
	set_reg(HL, addr - 1);
	cost(1, 8);
}
void Instructions::ld_a_hli() {
	uint16_t addr = get_reg(HL);
	uint8_t  val  = read_byte(addr);
	set_reg(A, val);
	set_reg(HL, addr + 1);
	cost(1, 8);
}
void Instructions::ld_a_hld() {
	uint16_t addr = get_reg(HL);
	uint8_t  val  = read_byte(addr);
	set_reg(A, val);
	set_reg(HL, addr - 1);
	cost(1, 8);
}
void Instructions::ld_hl_n8() {
	uint16_t addr = get_reg(HL);
	uint8_t  val  = fetch_byte();
	write_byte(addr, val);
	cost(2, 12);
}
void Instructions::ldh_a8_a() {
	uint8_t  a8   = fetch_byte();
	uint16_t addr = 0xFF00 + a8;
	uint8_t  val  = get_reg(A);
	write_byte(addr, val);
	cost(2, 12);
}
void Instructions::ldh_a_a8() {
	uint8_t  a8   = fetch_byte();
	uint16_t addr = 0xFF00 + a8;
	uint8_t  val  = read_byte(addr);
	set_reg(A, val);
	cost(2, 12);
}
void Instructions::ld_a16_a() {
	uint16_t addr = fetch_address();
	uint8_t  val  = get_reg(A);
	write_byte(addr, val);
	cost(3, 16);
}
void Instructions::ld_a_a16() {
	uint16_t addr = fetch_address();
	uint8_t  val  = read_byte(addr);
	set_reg(A, val);
	cost(3, 16);
}
void Instructions::ld_r8_n8(Reg reg) {
	uint8_t val = fetch_byte();
	set_reg(reg, val);
	cost(2, 8);
}
void Instructions::ld_r8_r8(Reg reg1, Reg reg2) {
	set_reg(reg1, get_reg(reg2));
	cost(1, 4);
}
void Instructions::add_hl_r16(Reg reg) {
	uint16_t val = get_reg(reg);
	set_flag(N, false);
	half_carry_on_add(get_reg(HL), val);
	set_flag(CY, (get_reg(HL) + val) > 0xFFFF);
	set_reg(HL, get_reg(HL) + val);
	cost(1, 8);
}
void Instructions::add_sp_e8() {
	uint16_t sp     = get_reg(SP);
	int8_t   e8     = fetch_signed_byte();
	int16_t  e8_ext = static_cast<int16_t>(e8);
	set_flag(Z, false);
	set_flag(N, false);
	set_flag(HC, ((sp & 0x000F) + (e8_ext & 0x000F)) & 0x0010);
	set_flag(CY, ((uint32_t)sp + (uint32_t)e8_ext) > 0xFFFF);
	set_reg(SP, sp + e8_ext);
	cost(2, 16);
}
void Instructions::dec_r16(Reg reg) {
	set_reg(reg, get_reg(reg) - 1);
	cost(1, 8);
}
void Instructions::inc_r16(Reg reg) {
	set_reg(reg, get_reg(reg) + 1);
	cost(1, 8);
}
void Instructions::inc_r8(Reg reg) {
	uint8_t r8;
	r8 = get_reg(reg);
	uint8_t res = r8 + 1;
	set_flag(Z, res == 0);
	set_flag(N, false);
	half_carry_on_add(r8, 1);
	set_reg(reg, res);
	cost(1, 4);
}
void Instructions::inc_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  val  = read_byte(addr);
	uint8_t  res  = val + 1;
	set_flag(Z, res == 0);
	set_flag(N, false);
	half_carry_on_add(val, 1);
	write_byte(addr, res);
	cost(1, 12);
}
void Instructions::dec_r8(Reg reg) {
	uint8_t r8;
	r8 = get_reg(reg);
	uint8_t res = r8 - 1;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(r8, 1);
	set_reg(reg, res);
	cost(1, 4);
}
void Instructions::dec_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  val  = read_byte(addr);
	uint8_t  res  = val - 1;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(val, 1);
	write_byte(addr, res);
	cost(1, 12);
}
void Instructions::add_a_r8(Reg reg) {
	uint8_t a   = get_reg(A);
	uint8_t r8  = get_reg(reg);
	uint8_t res = a + r8;
	auto    sum = static_cast<uint16_t>(a) + static_cast<uint16_t>(r8);
	set_flag(Z, res == 0);
	set_flag(N, false);
	half_carry_on_add(a, r8);
	set_flag(CY, sum > 0xFF);
	set_reg(A, res);
	cost(1, 4);
}
void Instructions::add_a_hl() {
	uint8_t  a    = get_reg(A);
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint8_t  res  = a + r8;
	auto     sum  = static_cast<uint16_t>(a) + static_cast<uint16_t>(r8);
	set_flag(Z, res == 0);
	set_flag(N, false);
	half_carry_on_add(a, r8);
	set_flag(CY, sum > 0xFF);
	set_reg(A, res);
	cost(1, 8);
}
void Instructions::add_a_n8() {
	uint8_t a   = get_reg(A);
	uint8_t n8  = fetch_byte();
	auto    sum = static_cast<uint16_t>(a) + static_cast<uint16_t>(n8);
	uint8_t res = a + n8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	half_carry_on_add(a, n8);
	set_flag(CY, sum > 0xFF);
	set_reg(A, res);
	cost(2, 8);
}
void Instructions::adc_a_r8(Reg reg) {
	uint8_t a    = get_reg(A);
	uint8_t r8   = get_reg(reg);
	uint8_t temp = r8 + get_flag(CY);
	auto    sum  = static_cast<uint16_t>(a) + static_cast<uint16_t>(temp);
	uint8_t res  = a + temp;
	set_flag(Z, res == 0);
	set_flag(N, false);
	half_carry_on_add(a, temp);
	set_flag(CY, (sum > 0xFF));
	set_reg(A, res);
	cost(1, reg == HL ? 8 : 4);
}
void Instructions::adc_a_hl() {
	uint8_t  a    = get_reg(A);
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint8_t  temp = r8 + get_flag(CY);
	auto     sum  = static_cast<uint16_t>(a) + static_cast<uint16_t>(temp);
	uint8_t  res  = a + temp;
	set_flag(Z, res == 0);
	set_flag(N, false);
	half_carry_on_add(a, temp);
	set_flag(CY, (sum > 0xFF));
	set_reg(A, res);
	cost(2, 8);
}
void Instructions::adc_a_n8() {
	uint8_t a    = get_reg(A);
	uint8_t n8   = fetch_byte();
	uint8_t temp = n8 + get_flag(CY);
	auto    sum  = static_cast<uint16_t>(a) + static_cast<uint16_t>(temp);
	uint8_t res  = a + temp;
	set_flag(Z, res == 0);
	set_flag(N, false);
	half_carry_on_add(a, temp);
	set_flag(CY, (sum > 0xFF));
	set_reg(A, res);
	cost(2, 8);
}
void Instructions::sub_a_r8(Reg reg) {
	uint8_t a   = get_reg(A);
	uint8_t r8  = get_reg(reg);
	uint8_t res = a - r8;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(a, r8);
	set_flag(CY, a < r8);
	set_reg(A, res);
	cost(1, 4);
}
void Instructions::sub_a_hl() {
	uint8_t  a    = get_reg(A);
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint8_t  res  = a - r8;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(a, r8);
	set_flag(CY, a < r8);
	set_reg(A, res);
	cost(1, 8);
}
void Instructions::sub_a_n8() {
	uint8_t a   = get_reg(A);
	uint8_t n8  = fetch_byte();
	uint8_t res = a - n8;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(a, n8);
	set_flag(CY, a < n8);
	set_reg(A, res);
	cost(2, 8);
}
void Instructions::sbc_a_r8(Reg reg) {
	uint8_t a    = get_reg(A);
	uint8_t r8   = get_reg(reg);
	uint8_t temp = r8 + get_flag(CY);
	uint8_t res  = a - temp;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(a, temp);
	set_flag(CY, a < temp);
	set_reg(A, res);
	cost(1, reg == HL ? 8 : 4);
}
void Instructions::sbc_a_hl() {
	uint8_t  a    = get_reg(A);
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint8_t  temp = r8 + get_flag(CY);
	uint8_t  res  = a - temp;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(a, temp);
	set_flag(CY, a < temp);
	set_reg(A, res);
	cost(2, 8);
}
void Instructions::sbc_a_n8() {
	uint8_t a    = get_reg(A);
	uint8_t n8   = fetch_byte();
	uint8_t temp = n8 + get_flag(CY);
	uint8_t res  = a - temp;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(a, temp);
	set_flag(CY, a < temp);
	set_reg(A, res);
	cost(2, 8);
}
void Instructions::and_a_r8(Reg reg) {
	uint8_t a   = get_reg(A);
	uint8_t r8  = get_reg(reg);
	uint8_t res = a & r8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, true);
	set_flag(CY, false);
	set_reg(A, res);
	cost(1, 4);
}
void Instructions::and_a_hl() {
	uint8_t  a    = get_reg(A);
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint8_t  res  = a & r8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, true);
	set_flag(CY, false);
	set_reg(A, res);
	cost(1, 8);
}
void Instructions::and_a_n8() {
	uint8_t a   = get_reg(A);
	uint8_t n8  = fetch_byte();
	uint8_t res = a & n8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, true);
	set_flag(CY, false);
	set_reg(A, res);
	cost(2, 8);
}
void Instructions::xor_a_r8(Reg reg) {
	uint8_t a   = get_reg(A);
	uint8_t r8  = get_reg(reg);
	uint8_t res = a ^ r8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, false);
	set_reg(A, res);
	cost(1, 4);
}
void Instructions::xor_a_hl() {
	uint8_t  a    = get_reg(A);
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint8_t  res  = a ^ r8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, false);
	set_reg(A, res);
	cost(1, 8);
}
void Instructions::xor_a_n8() {
	uint8_t a   = get_reg(A);
	uint8_t n8  = fetch_byte();
	uint8_t res = a ^ n8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, false);
	set_reg(A, res);
	cost(2, 8);
}
void Instructions::or_a_r8(Reg reg) {
	uint8_t a   = get_reg(A);
	uint8_t r8  = get_reg(reg);
	uint8_t res = a | r8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, false);
	set_reg(A, res);
	cost(1, 4);
}
void Instructions::or_a_hl() {
	uint8_t  a    = get_reg(A);
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint8_t  res  = a | r8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, false);
	set_reg(A, res);
	cost(1, 8);
}
void Instructions::or_a_n8() {
	uint8_t a   = get_reg(A);
	uint8_t n8  = fetch_byte();
	uint8_t res = a | n8;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, false);
	set_reg(A, res);
	cost(2, 8);
}
void Instructions::cp_a_r8(Reg reg) {
	uint8_t a   = get_reg(A);
	uint8_t r8  = get_reg(reg);
	uint8_t res = a - r8;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(a, r8);
	set_flag(CY, a < r8);
	cost(1, 4);
}
void Instructions::cp_a_hl() {
	uint8_t  a    = get_reg(A);
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint8_t  res  = a - r8;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(a, r8);
	set_flag(CY, a < r8);
	cost(1, 8);
}
void Instructions::cp_a_n8() {
	uint8_t a   = get_reg(A);
	uint8_t n8  = fetch_byte();
	uint8_t res = a - n8;
	set_flag(Z, res == 0);
	set_flag(N, true);
	half_carry_on_sub(a, n8);
	set_flag(CY, a < n8);
	cost(2, 8);
}
void Instructions::daa() {
	// source:
	// https://forums.nesdev.org/viewtopic.php?p=196282&sid=c09957170f76c60f51bc270df9c829e5#p196282
	// Retroactive BCD addition/subtraction adjustments
	if (!get_flag(N)) {
		// Adjustments after an addition.
		if (get_flag(HC) || (get_reg(A) & 0x0F) > 0x09) {
			set_reg(A, get_reg(A) + 0x06);
		}

		if (get_flag(CY) || get_reg(A) > 0x99) {
			set_reg(A, get_reg(A) + 0x60);
			set_flag(CY, true);
		}
	} else {
		// Adjustments after a subtraction.
		if (get_flag(HC)) {
			set_reg(A, get_reg(A) - 0x6);
		}

		if (get_flag(CY)) {
			set_reg(A, get_reg(A) - 0x60);
		}
	}

	set_flag(Z, get_reg(A) == 0);
	set_flag(HC, false);
	cost(1, 4);
}
void Instructions::cpl() {
	set_reg(A, ~get_reg(A));
	set_flag(N, true);
	set_flag(HC, true);
	cost(1, 4);
}
void Instructions::scf() {
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, true);
	cost(1, 4);
}
void Instructions::ccf() {
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, !get_flag(CY));
	cost(1, 4);
}
void Instructions::rlc_r8(Reg reg) {
	uint8_t  r8  = get_reg(reg);
	uint16_t res = (r8 << 1) | (r8 >> 7);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x80) == 0x80);
	cost(2, 8);
}
void Instructions::rlc_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint16_t res  = (r8 << 1) | (r8 >> 7);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x80) == 0x80);
	write_byte(addr, res);
	cost(2, 16);
}
void Instructions::rl_r8(Reg reg) {
	uint8_t  r8  = get_reg(reg);
	uint16_t res = (r8 << 1) | get_flag(CY);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x80) == 0x80);
	set_reg(reg, res);
	cost(2, 8);
}
void Instructions::rl_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint16_t res  = (r8 << 1) | get_flag(CY);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x80) == 0x80);
	write_byte(addr, res);
	cost(2, 16);
}
void Instructions::rrc_r8(Reg reg) {
	uint8_t  r8  = get_reg(reg);
	uint16_t res = (r8 >> 1) | (r8 << 7);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x01) == 0x01);
	set_reg(reg, res);
	cost(2, 8);
}
void Instructions::rrc_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint16_t res  = (r8 >> 1) | (r8 << 7);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x01) == 0x01);
	write_byte(addr, res);
	cost(2, 16);
}
void Instructions::rr_r8(Reg reg) {
	uint8_t  r8  = get_reg(reg);
	uint16_t res = (r8 >> 1) | (get_flag(CY) << 7);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x01) == 0x01);
	set_reg(reg, res);
	cost(2, 8);
}
void Instructions::rr_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint16_t res  = (r8 >> 1) | (get_flag(CY) << 7);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x01) == 0x01);
	write_byte(addr, res);
	cost(2, 16);
}
void Instructions::sla_r8(Reg reg) {
	uint8_t  r8  = get_reg(reg);
	uint16_t res = r8 << 1;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x80) == 0x80);
	set_reg(reg, res);
	cost(2, 8);
}
void Instructions::sla_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint16_t res  = r8 << 1;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x80) == 0x80);
	write_byte(addr, res);
	cost(2, 16);
}
void Instructions::sra_r8(Reg reg) {
	uint8_t  r8  = get_reg(reg);
	uint16_t res = (r8 >> 1) | (r8 & 0x80);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x01) == 0x01);
	set_reg(reg, res);
	cost(2, 8);
}
void Instructions::sra_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint16_t res  = (r8 >> 1) | (r8 & 0x80);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x01) == 0x01);
	write_byte(addr, res);
	cost(2, 16);
}
void Instructions::swap_r8(Reg reg) {
	uint8_t  r8  = get_reg(reg);
	uint16_t res = (r8 << 4) | (r8 >> 4);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, false);
	set_reg(reg, res);
	cost(2, 8);
}
void Instructions::swap_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint16_t res  = (r8 << 4) | (r8 >> 4);
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, false);
	write_byte(addr, res);
	cost(2, 16);
}
void Instructions::srl_r8(Reg reg) {
	uint8_t  r8  = get_reg(reg);
	uint16_t res = r8 >> 1;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x01) == 0x01);
	set_reg(reg, res);
	cost(2, 8);
}
void Instructions::srl_hl() {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	uint16_t res  = r8 >> 1;
	set_flag(Z, res == 0);
	set_flag(N, false);
	set_flag(HC, false);
	set_flag(CY, (r8 & 0x01) == 0x01);
	write_byte(addr, res);
	cost(2, 16);
}
void Instructions::bit_r8(Reg reg, uint8_t bit) {
	uint8_t r8 = get_reg(reg);
	set_flag(Z, (r8 & (1 << bit)) == 0);
	set_flag(N, false);
	set_flag(HC, true);
	cost(2, 8);
}
void Instructions::bit_hl(uint8_t bit) {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	set_flag(Z, (r8 & (1 << bit)) == 0);
	set_flag(N, false);
	set_flag(HC, true);
	cost(2, 16);
}
void Instructions::res_r8(Reg reg, uint8_t bit) {
	uint8_t r8 = get_reg(reg);
	r8 &= ~(1 << bit);
	set_reg(reg, r8);
	cost(2, 8);
}
void Instructions::res_hl(uint8_t bit) {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	r8 &= ~(1 << bit);
	write_byte(addr, r8);
	cost(2, 16);
}
void Instructions::set_r8(Reg reg, uint8_t bit) {
	uint8_t r8 = get_reg(reg);
	r8 |= (1 << bit);
	set_reg(reg, r8);
	cost(2, 8);
}
void Instructions::set_hl(uint8_t bit) {
	uint16_t addr = get_reg(HL);
	uint8_t  r8   = read_byte(addr);
	r8 |= (1 << bit);
	write_byte(addr, r8);
	cost(2, 16);
}