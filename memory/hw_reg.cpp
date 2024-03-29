#include "hw_reg.h"

uint8_t HardwareRegisters::get_vbk() const {
	return VBK & 0x1; // Only bit 0 is used.
}
uint8_t HardwareRegisters::get_svbk() const {
	return SVBK & 0x7; // Only 3 bits are used.
}
uint8_t HardwareRegisters::get_div() const {
	return DIV;
}
uint8_t HardwareRegisters::get_tima() const {
	return TIMA;
}
uint8_t HardwareRegisters::get_tma() const {
	return TMA;
}
uint8_t HardwareRegisters::get_tac() const {
	return TAC;
}
uint8_t HardwareRegisters::get_if() const {
	return IF;
}
uint8_t HardwareRegisters::get_key1() const {
	return KEY1;
}
uint8_t HardwareRegisters::get_ie() const {
	return IE;
}
void HardwareRegisters::set_vbk(uint8_t val) {
	VBK = val & 0x1; // Only bit 0 is used.
}
void HardwareRegisters::set_svbk(uint8_t val) {
	SVBK = val & 0x7; // Only 3 bits are used.
}
void HardwareRegisters::set_div(uint8_t val) {
	DIV = val;
}
void HardwareRegisters::set_tima(uint8_t val) {
	TIMA = val;
}
void HardwareRegisters::set_tma(uint8_t val) {
	TMA = val;
}
void HardwareRegisters::set_tac(uint8_t val) {
	TAC = val;
}
void HardwareRegisters::set_if(uint8_t val) {
	IF = val & 0x1F; // Only 5 bits are used.
}
void HardwareRegisters::set_key1(uint8_t val) {
	KEY1 = val & 0x1; // Only bit 0 is used.
}
void HardwareRegisters::set_ie(uint8_t val) {
	IE = val & 0x1F; // Only 5 bits are used.
}