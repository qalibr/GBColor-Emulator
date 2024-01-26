#include "operator.h"

uint16_t Operator::get_reg(Registers reg) {
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
void Operator::set_reg(Registers reg, uint16_t val) {
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
	default:
		throw std::runtime_error("Invalid flag");
	}
}