#ifndef AYB_MEM_MAP_H
#define AYB_MEM_MAP_H

#include "cstdint"

struct MemoryMap
{
	static constexpr uint16_t ROM_FIX_START  = 0x0000;
	static constexpr uint16_t ROM_FIX_END    = 0x3FFF;
	static constexpr uint16_t ROM_SW_START   = 0x4000;
	static constexpr uint16_t ROM_SW_END     = 0x7FFF;
	static constexpr uint16_t VRAM_START     = 0x8000;
	static constexpr uint16_t VRAM_END       = 0x9FFF;
	static constexpr uint16_t EXT_RAM_START  = 0xA000;
	static constexpr uint16_t EXT_RAM_END    = 0xBFFF;
	static constexpr uint16_t WRAM_FIX_START = 0xC000;
	static constexpr uint16_t WRAM_FIX_END   = 0xCFFF;
	static constexpr uint16_t WRAM_SW_START  = 0xD000;
	static constexpr uint16_t WRAM_SW_END    = 0xDFFF;
	static constexpr uint16_t ECHO_START     = 0xE000;
	static constexpr uint16_t ECHO_END       = 0xFDFF;
	static constexpr uint16_t OAM_START      = 0xFE00;
	static constexpr uint16_t OAM_END        = 0xFE9F;
	static constexpr uint16_t UNUSED_START   = 0xFEA0;
	static constexpr uint16_t UNUSED_END     = 0xFEFF;
	static constexpr uint16_t IO_START       = 0xFF00;
	static constexpr uint16_t IO_END         = 0xFF7F;
	static constexpr uint16_t HRAM_START     = 0xFF80;
	static constexpr uint16_t HRAM_END       = 0xFFFE; // Not really end, but will handle certain regs differently.
};

#endif //AYB_MEM_MAP_H
