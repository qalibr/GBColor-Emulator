#include "operator.h"

void OpcodeMap::init_instructions() {
	instructions[0x00] = [this](Reg reg) { this->nop(); };
}

void OpcodeMap::execute(uint8_t op_code) {
	if (instructions[op_code]) {
		instructions[op_code];
		return;
	}
	else {
		cb_instructions[op_code];
		return;
	}

	throw std::runtime_error("Invalid opcode");
}