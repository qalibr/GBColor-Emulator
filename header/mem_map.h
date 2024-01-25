#ifndef AYB_MEM_MAP_H
#define AYB_MEM_MAP_H

#include <cstdint>

struct MemoryMap
{
	static constexpr uint16_t ROM_FIX        = 0x3FFF;
	static constexpr uint16_t ROM_SW         = 0x7FFF;
	static constexpr uint16_t VRAM_BANK      = 0x9FFF;
	static constexpr uint16_t ERAM_BANK      = 0xBFFF;
	static constexpr uint16_t WRAM_FIX_START = 0xC000;
	static constexpr uint16_t WRAM_FIX_END   = 0xCFFF;
	static constexpr uint16_t WRAM_SW_START  = 0xD000;
	static constexpr uint16_t WRAM_SW_END    = 0xDFFF;
	static constexpr uint16_t ECHO           = 0xFDFF;
	static constexpr uint16_t OAM            = 0xFE9F;
	static constexpr uint16_t UNUSED         = 0xFEFF;
	static constexpr uint16_t IO             = 0xFF7F;
	static constexpr uint16_t HRAM           = 0xFFFE;
};

#endif //AYB_MEM_MAP_H
