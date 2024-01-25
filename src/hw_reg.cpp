#include "../header/hw_reg.h"

uint8_t HardwareRegisters::get_vbk() const
{
	return VBK & 0x1;
}

void HardwareRegisters::set_vbk(uint8_t val)
{
	VBK = val & 0x1;
}