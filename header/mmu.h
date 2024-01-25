#ifndef AYB_MMU_H
#define AYB_MMU_H

#include <cstdint>
#include "IMmu.h"

class Mmu : public IMmu
{
public:
	Mmu();
	uint8_t read_byte(uint16_t adr) override;
	void write_byte(uint16_t adr, uint8_t val) override;
	uint16_t read_word(uint16_t adr) override;
	void write_word(uint16_t adr, uint16_t val) override;
};

#endif //AYB_MMU_H
