#include "cpu_flag.h"

bool CpuFlags::get_z() const {
	return (cpu_reg.get_f() & Z_FLAG) != 0;
}
bool CpuFlags::get_n() const {
	return (cpu_reg.get_f() & N_FLAG) != 0;
}
bool CpuFlags::get_hc() const {
	return (cpu_reg.get_f() & HC_FLAG) != 0;
}
bool CpuFlags::get_cy() const {
	return (cpu_reg.get_f() & CY_FLAG) != 0;
}
bool CpuFlags::get_ime() const {
	return IME;
}
bool CpuFlags::get_halt() const {
	return HALT;
}
bool CpuFlags::get_stop() const {
	return STOP;
}
bool CpuFlags::get_enable_interrupts() const {
	return enable_interrupts;
}
bool CpuFlags::get_enable_interrupts_pending() const {
	return enable_interrupts_pending;
}
void CpuFlags::set_z(bool val) {
	F = val ? F | Z_FLAG : F & ~Z_FLAG;
}
void CpuFlags::set_n(bool val) {
	F = val ? F | N_FLAG : F & ~N_FLAG;
}
void CpuFlags::set_hc(bool val) {
	F = val ? F | HC_FLAG : F & ~HC_FLAG;
}
void CpuFlags::set_cy(bool val) {
	F = val ? F | CY_FLAG : F & ~CY_FLAG;
}
void CpuFlags::set_ime(bool val) {
	IME = val;
}
void CpuFlags::set_halt(bool val) {
	HALT = val;
}
void CpuFlags::set_stop(bool val) {
	STOP = val;
}
void CpuFlags::set_enable_interrupts(bool val) {
	enable_interrupts = val;
}
void CpuFlags::set_enable_interrupts_pending(bool val) {
	enable_interrupts_pending = val;
}