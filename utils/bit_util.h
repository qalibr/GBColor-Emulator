#ifndef AYB_BIT_UTIL_H
#define AYB_BIT_UTIL_H

#include "cstdint"

class BitUtil {
public:
	static bool is_bit_set(uint8_t B, int b);
	static uint8_t set_bit(uint8_t B, int b);
	static uint8_t clear_bit(uint8_t B, int b);
	static uint8_t toggle_bit(uint8_t B, int b);
};

#endif //AYB_BIT_UTIL_H
