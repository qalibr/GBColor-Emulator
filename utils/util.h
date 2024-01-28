#ifndef AYB_UTIL_H
#define AYB_UTIL_H

#include "cstdint"
#include "string"

class Util {
public:
	static std::string opcode_to_hex(uint8_t opcode) {
		char buf[3];
		sprintf(buf, "%02X", opcode);
		return buf;
	}
};


#endif //AYB_UTIL_H
