#ifndef AYB_MEM_MAP_H
#define AYB_MEM_MAP_H

#include "cstdint"

struct MemoryMap
{
	static constexpr uint16_t ROM_FIX_END    = 0x3FFF;
	static constexpr uint16_t ROM_SW_END     = 0x7FFF;
	static constexpr uint16_t VRAM_BANK_END  = 0x9FFF;
	static constexpr uint16_t ERAM_BANK_END  = 0xBFFF;
	static constexpr uint16_t WRAM_FIX_START = 0xC000;
	static constexpr uint16_t WRAM_FIX_END   = 0xCFFF;
	static constexpr uint16_t WRAM_SW_START  = 0xD000;
	static constexpr uint16_t WRAM_SW_END    = 0xDFFF;
	static constexpr uint16_t ECHO_END       = 0xFDFF;
	static constexpr uint16_t OAM_END        = 0xFE9F;
	static constexpr uint16_t UNUSED_END     = 0xFEFF;
	static constexpr uint16_t IO_END         = 0xFF7F;
	static constexpr uint16_t HRAM_END       = 0xFFFE; // Not really end, but will handle certain regs differently.
};

#endif //AYB_MEM_MAP_H
