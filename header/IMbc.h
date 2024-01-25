#ifndef AYB_IMBC_H
#define AYB_IMBC_H

#include "cstdint"

class IMbc {
public:
	virtual ~IMbc() = default;

	virtual uint8_t read_rom_high(uint16_t addr) = 0;
	virtual uint8_t read_rom_low(uint16_t addr) = 0;
	virtual void write_rom(uint16_t addr, uint8_t val) = 0;
	virtual uint8_t read_ram(uint16_t addr) = 0;
	virtual void write_ram(uint16_t addr, uint8_t val) = 0;
};

#endif //AYB_IMBC_H
