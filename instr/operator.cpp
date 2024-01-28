#include "operator.h"

uint16_t Operator::get_reg(Reg reg) {
	switch (reg) {
	case A:
		return cpu.get_cpu_reg().get_a();
	case F:
		return cpu.get_cpu_reg().get_f();
	case B:
		return cpu.get_cpu_reg().get_b();
	case C:
		return cpu.get_cpu_reg().get_c();
	case D:
		return cpu.get_cpu_reg().get_d();
	case E:
		return cpu.get_cpu_reg().get_e();
	case H:
		return cpu.get_cpu_reg().get_h();
	case L:
		return cpu.get_cpu_reg().get_l();
	case AF:
		return cpu.get_cpu_reg().get_af();
	case BC:
		return cpu.get_cpu_reg().get_bc();
	case DE:
		return cpu.get_cpu_reg().get_de();
	case HL:
		return cpu.get_cpu_reg().get_hl();
	case SP:
		return cpu.get_cpu_reg().get_sp();
	case PC:
		return cpu.get_cpu_reg().get_pc();
	default:
		throw std::runtime_error("Invalid register");
	}
}
void Operator::set_reg(Reg reg, uint16_t val) {
	switch (reg) {

		/* 8-bit register - handling assign value */
	case A:
		cpu.get_cpu_reg().set_a(val & 0xFF);
		break;
	case F:
		cpu.get_cpu_reg().set_f(val & 0xFF);
		break;
	case B:
		cpu.get_cpu_reg().set_b(val & 0xFF);
		break;
	case C:
		cpu.get_cpu_reg().set_c(val & 0xFF);
		break;
	case D:
		cpu.get_cpu_reg().set_d(val & 0xFF);
		break;
	case E:
		cpu.get_cpu_reg().set_e(val & 0xFF);
		break;
	case H:
		cpu.get_cpu_reg().set_h(val & 0xFF);
		break;
	case L:
		cpu.get_cpu_reg().set_l(val & 0xFF);
		break;

		/* 16-bit registers */
	case AF:
		cpu.get_cpu_reg().set_af(val);
		break;
	case BC:
		cpu.get_cpu_reg().set_bc(val);
		break;
	case DE:
		cpu.get_cpu_reg().set_de(val);
		break;
	case HL:
		cpu.get_cpu_reg().set_hl(val);
		break;
	case SP:
		cpu.get_cpu_reg().set_sp(val);
		break;
	case PC:
		cpu.get_cpu_reg().set_pc(val);
		break;
	default:
		throw std::runtime_error("Invalid register");
	}
}
uint8_t Operator::get_flag(Flags flag) {
	switch (flag) {
	case Z:
		return cpu.get_cpu_flag().get_z();
	case N:
		return cpu.get_cpu_flag().get_n();
	case HC:
		return cpu.get_cpu_flag().get_hc();
	case CY:
		return cpu.get_cpu_flag().get_cy();
	case IME:
		return cpu.get_cpu_flag().get_ime();
	case HALT:
		return cpu.get_cpu_flag().get_halt();
	case STOP:
		return cpu.get_cpu_flag().get_stop();
	default:
		throw std::runtime_error("Invalid flag");
	}
}
void Operator::set_flag(Flags flag, uint8_t val) {
	switch (flag) {
	case Z:
		cpu.get_cpu_flag().set_z(val);
		break;
	case N:
		cpu.get_cpu_flag().set_n(val);
		break;
	case HC:
		cpu.get_cpu_flag().set_hc(val);
		break;
	case CY:
		cpu.get_cpu_flag().set_cy(val);
		break;
	case IME:
		cpu.get_cpu_flag().set_ime(val);
		break;
	case HALT:
		cpu.get_cpu_flag().set_halt(val);
		break;
	case STOP:
		cpu.get_cpu_flag().set_stop(val);
		break;
	default:
		throw std::runtime_error("Invalid flag");
	}
}
uint8_t Operator::fetch_byte() {
	uint16_t pc = get_reg(PC);
	return cpu.get_mmu().read_byte(pc);
//	set_reg(PC, pc + 1); // TODO: Incrementing PC here, make sure this works as intended with the cost function.
//	return byte;
}
int8_t Operator::fetch_signed_byte() {
	return static_cast<int8_t>(fetch_byte());
}
uint16_t Operator::fetch_a8_address() {
	uint8_t lower = fetch_byte();
	return 0xFF00 | lower;
}
uint16_t Operator::fetch_address() {
	uint16_t lower = fetch_byte();
	uint16_t upper = fetch_byte();
	return (upper << 8) | lower;
}
uint8_t Operator::read_byte(uint16_t addr) {
	return cpu.get_mmu().read_byte(addr);
}
void Operator::write_byte(uint16_t addr, uint8_t byte) {
	cpu.get_mmu().write_byte(addr, byte);
}