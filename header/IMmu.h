#ifndef AYB_IMMU_H
#define AYB_IMMU_H

#include <cstdint>

class IMmu
{
public:
	virtual uint8_t read_byte(uint16_t adr) = 0;
	virtual void write_byte(uint16_t adr, uint8_t val) = 0;
	virtual uint16_t read_word(uint16_t adr) = 0;
	virtual void write_word(uint16_t adr, uint16_t val) = 0;
	virtual ~IMmu() = default; // Default cleanup in child classes.
};

#endif //AYB_IMMU_H
