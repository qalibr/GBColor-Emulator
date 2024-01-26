#include "cpu_reg.h"

CpuRegisters::CpuRegisters() {
	A  = 0x01;
	F  = 0xB0;
	B  = 0x00;
	C  = 0x13;
	D  = 0x00;
	E  = 0xD8;
	H  = 0x01;
	L  = 0x4D;
	SP = 0xFFFE;
	PC = 0x0100;
}
uint8_t CpuRegisters::get_a() const {
	return A;
}
uint8_t CpuRegisters::get_f() const {
	return F;
}
uint8_t CpuRegisters::get_b() const {
	return B;
}
uint8_t CpuRegisters::get_c() const {
	return C;
}
uint8_t CpuRegisters::get_d() const {
	return D;
}
uint8_t CpuRegisters::get_e() const {
	return E;
}
uint8_t CpuRegisters::get_h() const {
	return H;
}
uint8_t CpuRegisters::get_l() const {
	return L;
}
uint16_t CpuRegisters::get_af() const {
	return A << 8 | F;
}
uint16_t CpuRegisters::get_bc() const {
	return B << 8 | C;
}
uint16_t CpuRegisters::get_de() const {
	return D << 8 | E;
}
uint16_t CpuRegisters::get_hl() const {
	return H << 8 | L;
}
uint16_t CpuRegisters::get_sp() const {
	return SP;
}
uint16_t CpuRegisters::get_pc() const {
	return PC;
}
void CpuRegisters::set_a(uint8_t val) {
	A = val;
}
void CpuRegisters::set_f(uint8_t val) {
	F = val & 0xF0; // Lower nibble is reserved for flags.
}
void CpuRegisters::set_b(uint8_t val) {
	B = val;
}
void CpuRegisters::set_c(uint8_t val) {
	C = val;
}
void CpuRegisters::set_d(uint8_t val) {
	D = val;
}
void CpuRegisters::set_e(uint8_t val) {
	E = val;
}
void CpuRegisters::set_h(uint8_t val) {
	H = val;
}
void CpuRegisters::set_l(uint8_t val) {
	L = val;
}
void CpuRegisters::set_af(uint16_t val) {
	A = val >> 8;
	F = val & 0xF0;
}
void CpuRegisters::set_bc(uint16_t val) {
	B = val >> 8;
	C = val & 0xFF;
}
void CpuRegisters::set_de(uint16_t val) {
	D = val >> 8;
	E = val & 0xFF;
}
void CpuRegisters::set_hl(uint16_t val) {
	H = val >> 8;
	L = val & 0xFF;
}
void CpuRegisters::set_sp(uint16_t val) {
	SP = val;
}
void CpuRegisters::add_sp(uint8_t val) {
	SP += val;
}
void CpuRegisters::dec_sp(uint8_t val) {
	SP -= val;
}
void CpuRegisters::set_pc(uint16_t val) {
	PC = val;
}
void CpuRegisters::add_pc(uint8_t val) {
	PC += val;
}
void CpuRegisters::dec_pc(uint8_t val) {
	PC -= val;
}
