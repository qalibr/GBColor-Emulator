#ifndef AYB_MMU_H
#define AYB_MMU_H

#include "cstdint"

#include "IMmu.h"
#include "IMbc.h"

// MBC: IMbc is implemented elsewhere, but it is used here.
//
// MMU: Implementing basic memory read/write functions here,
// other classes will use IMmu to access read/write functions.
//
// Memory:
// The Memory class will supply various getters/setters and
// in general all useful helper functions to do with memory.
// I'm considering making a memory utility class.

class Mmu : public IMmu
{
private:
	IMbc* mbc;

public:
	Mmu();
	uint8_t read_byte(uint16_t adr) override;
	void write_byte(uint16_t adr, uint8_t val) override;
	uint16_t read_word(uint16_t adr) override;
	void write_word(uint16_t adr, uint16_t val) override;
};

#endif //AYB_MMU_H
