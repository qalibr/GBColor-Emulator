#include "../header/mem.h"

uint8_t* Memory::get_vram_bank(uint16_t addr, int bank)
{
	if (hw_reg.get_vbk() == 0)
	{
		return &VRAM0[addr - MemoryMap::VRAM_BANK];
	}
	else
	{
		return &VRAM1[addr - MemoryMap::VRAM_BANK];
	}
}