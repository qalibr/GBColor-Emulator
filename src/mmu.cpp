#include "../header/mmu.h"

uint8_t Mmu::read_byte(uint16_t addr)
{
	switch (addr)
	{
	case MemoryMap::ROM_FIX_START ... MemoryMap::ROM_FIX_END:
		return mbc->read_rom_low(addr);
	case MemoryMap::ROM_SW_START ... MemoryMap::ROM_SW_END:
		return mbc->read_rom_high(addr);
	case MemoryMap::VRAM_BANK_START ... MemoryMap::VRAM_BANK_END:
		return *mem_util.read_vram_bank(addr);
	default:
		throw std::out_of_range("Address out of range");
	}
}