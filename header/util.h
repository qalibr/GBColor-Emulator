#ifndef AYB_UTIL_H
#define AYB_UTIL_H

#include <cstdint>

class Util
{
public:
	static bool is_bit_set(uint8_t B, int b);
	static uint8_t set_bit(uint8_t B, int b);
	static uint8_t clear_bit(uint8_t B, int b);
	static uint8_t toggle_bit(uint8_t B, int b);
};
bool Util::is_bit_set(uint8_t B, int b)
{
	return (B & (1 << b)) != 0;
}
uint8_t Util::set_bit(uint8_t B, int b)
{
	return B | (1 << b);
}
uint8_t Util::clear_bit(uint8_t B, int b)
{
	return B & ~(1 << b);
}
uint8_t Util::toggle_bit(uint8_t B, int b)
{
	return B ^ (1 << b);
}

#endif //AYB_UTIL_H
