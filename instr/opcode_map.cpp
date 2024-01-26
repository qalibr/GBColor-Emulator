#include "operator.h"

void OpcodeMap::init_instructions() {
	/* Arithmetic 16-bit instructions */
	instructions[0x03] = [this](Registers reg) { this->inc_16(BC); };
	instructions[0x13] = [this](Registers reg) { this->inc_16(DE); };
	instructions[0x23] = [this](Registers reg) { this->inc_16(HL); };
	instructions[0x33] = [this](Registers reg) { this->inc_16(SP); };

	instructions[0x09] = [this](Registers reg) { this->add_hl(BC); };
	instructions[0x19] = [this](Registers reg) { this->add_hl(DE); };
	instructions[0x29] = [this](Registers reg) { this->add_hl(HL); };
	instructions[0x39] = [this](Registers reg) { this->add_hl(SP); };

	instructions[0x0B] = [this](Registers reg) { this->dec_16(BC); };
	instructions[0x1B] = [this](Registers reg) { this->dec_16(DE); };
	instructions[0x2B] = [this](Registers reg) { this->dec_16(HL); };
	instructions[0x3B] = [this](Registers reg) { this->dec_16(SP); };
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