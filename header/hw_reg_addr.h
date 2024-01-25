#ifndef AYB_HW_REG_ADDR_H
#define AYB_HW_REG_ADDR_H

#include "cstdint"

struct HwRegAddr {
	static constexpr uint16_t LCDC  = 0xFF40; // R/W
	static constexpr uint16_t STAT  = 0xFF41; // R/W
	static constexpr uint16_t SCY   = 0xFF42; // R/W
	static constexpr uint16_t SCX   = 0xFF43; // R/W
	static constexpr uint16_t LY    = 0xFF44; // R
	static constexpr uint16_t LYC   = 0xFF45; // R/W
	static constexpr uint16_t DMA   = 0xFF46; // R/W
	static constexpr uint16_t WY    = 0xFF4A; // R/W
	static constexpr uint16_t WX    = 0xFF4B; // R/W
	static constexpr uint16_t KEY1  = 0xFF4D; // R/W
	static constexpr uint16_t VBK   = 0xFF4F; // R/W
	static constexpr uint16_t HDMA1 = 0xFF51; // W
	static constexpr uint16_t HDMA2 = 0xFF52; // W
	static constexpr uint16_t HDMA3 = 0xFF53; // W
	static constexpr uint16_t HDMA4 = 0xFF54; // W
	static constexpr uint16_t HDMA5 = 0xFF55; // R/W
	static constexpr uint16_t BCPS  = 0xFF68; // R/W
	static constexpr uint16_t BCPD  = 0xFF69; // R/W
	static constexpr uint16_t OCPS  = 0xFF6A; // R/W
	static constexpr uint16_t OCPD  = 0xFF6B; // R/W
	static constexpr uint16_t OPRI  = 0xFF6C; // R/W
	static constexpr uint16_t SVBK  = 0xFF70; // R/W
	static constexpr uint16_t IE    = 0xFFFF; // R/W
};

#endif //AYB_HW_REG_ADDR_H
