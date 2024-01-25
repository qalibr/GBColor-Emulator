#include "../header/mem.h"

uint8_t* Memory::read_vram_bank(uint16_t addr)
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
uint8_t* Memory::read_wram_bank_fix(uint16_t addr)
{
	if (addr <= MemoryMap::WRAM_FIX_START || addr >= MemoryMap::WRAM_FIX_END)
	{
		throw std::out_of_range("Fixed WRAM bank out of range");
	}

	return &WRAM0[addr - MemoryMap::WRAM_FIX_START];
}
uint8_t* Memory::read_wram_bank_sw(uint16_t addr, int bank)
{
	bank = hw_reg.get_svbk();
	if (bank == 0)
	{
		bank = 1; // 0 is interpreted as 1.
	}

	if (bank < 1 || bank > 7)
	{
		throw std::out_of_range("Invalid WRAM bank");
	}
	if (addr <= MemoryMap::WRAM_SW_START || addr >= MemoryMap::WRAM_SW_END)
	{
		throw std::out_of_range("WRAM bank address out of range");
	}

	return &WRAM_BANKS[bank][addr - MemoryMap::WRAM_SW_START];
}

void Memory::write_wram_bank_fix(uint16_t addr, uint8_t val)
{
	if (addr <= MemoryMap::WRAM_FIX_START || addr >= MemoryMap::WRAM_FIX_END)
	{
		throw std::out_of_range("Fixed WRAM bank out of range");
	}

	WRAM_BANKS[0][addr - MemoryMap::WRAM_FIX_START] = val;
}
void Memory::write_wram_bank_sw(uint16_t addr, uint8_t val)
{
	int bank = hw_reg.get_svbk();
	if (bank == 0)
	{
		bank = 1; // 0 is interpreted as 1.
	}

	if (bank < 1 || bank > 7)
	{
		throw std::out_of_range("Invalid WRAM bank");
	}
	if (addr <= MemoryMap::WRAM_SW_START || addr >= MemoryMap::WRAM_SW_END)
	{
		throw std::out_of_range("WRAM bank address out of range");
	}

	WRAM_BANKS[bank][addr - MemoryMap::WRAM_SW_START] = val;
}