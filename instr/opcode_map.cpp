#include "operator.h"

void OpcodeMap::init_instructions() {
	instructions[0x00] = [this]() { this->nop(); };

	instructions[0xCB] = [this]() { this->prefix_cb(); };
}
void OpcodeMap::execute(uint8_t op_code) {
	if (instructions[op_code]) {
		instructions[op_code]();
		return;
	}

	throw std::runtime_error("Invalid opcode");
}