#include "bit_util.h"

bool BitUtil::is_bit_set(uint8_t B, int b) {
	return (B & (1 << b)) != 0;
}
uint8_t BitUtil::set_bit(uint8_t B, int b) {
	return B | (1 << b);
}
uint8_t BitUtil::clear_bit(uint8_t B, int b) {
	return B & ~(1 << b);
}
uint8_t BitUtil::toggle_bit(uint8_t B, int b) {
	return B ^ (1 << b);
}
