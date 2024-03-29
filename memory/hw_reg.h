#ifndef AYB_HW_REG_H
#define AYB_HW_REG_H

#include "cstdint"

class HardwareRegisters {
private:
	uint8_t SVBK; // WRAM bank switch register. CGB only.
	uint8_t VBK;  // VRAM bank switch register. CGB only.
	uint8_t DIV;  //
	uint8_t TIMA; //
	uint8_t TMA;  //
	uint8_t TAC;  //
	uint8_t LCDC; // FF40
	uint8_t STAT; // FF41
	uint8_t SCY;  // FF42
	uint8_t SCX;  // FF43
	uint8_t LY;   // FF44
	uint8_t LYC;  // FF45
	uint8_t DMA;  // FF46
	uint8_t WY;   // FF4A
	uint8_t WX;   // FF4B
	uint8_t KEY1; // FF4D (CGB) Prepare speed switch
	uint8_t HDMA1; // FF51
	uint8_t HDMA2; // FF52
	uint8_t HDMA3; // FF53
	uint8_t HDMA4; // FF54
	uint8_t HDMA5; // FF55
	uint8_t BCPS; // FF68 (CGB)
	uint8_t BCPD; // FF69 (CGB)
	uint8_t OCPS; // FF6A (CGB)
	uint8_t OCPD; // FF6B (CGB)
	uint8_t OPRI; // FF6C (CGB)
	uint8_t IF;   // FF0F
	uint8_t IE;   // FF0F

public:
	HardwareRegisters() {
		VBK  = 0;
		KEY1 = 0x7E; // Normal speed mode and no pending speed switch
	}
	~HardwareRegisters() = default;

	[[nodiscard]] uint8_t get_vbk() const;
	[[nodiscard]] uint8_t get_svbk() const;
	[[nodiscard]] uint8_t get_div() const;
	[[nodiscard]] uint8_t get_tima() const;
	[[nodiscard]] uint8_t get_tma() const;
	[[nodiscard]] uint8_t get_tac() const;
	[[nodiscard]] uint8_t get_key1() const;
	[[nodiscard]] uint8_t get_if() const;
	[[nodiscard]] uint8_t get_ie() const;

	void set_vbk(uint8_t val);
	void set_svbk(uint8_t val);
	void set_div(uint8_t val);
	void set_tima(uint8_t val);
	void set_tma(uint8_t val);
	void set_tac(uint8_t val);
	void set_key1(uint8_t val);
	void set_if(uint8_t val);
	void set_ie(uint8_t val);
};

#endif //AYB_HW_REG_H
