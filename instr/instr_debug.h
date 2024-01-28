#ifndef AYB_INSTR_DEBUG_H
#define AYB_INSTR_DEBUG_H

#include <cstdint>
#include <stdexcept>
#include <cassert>

class InstructionDebug {
private:
	uint16_t initial_pc;

public:
	void set_initial_pc(uint16_t pc) { initial_pc = pc; }

	void assert_pc(uint16_t currentPc, uint16_t expectedOffset) {
		uint16_t expected_pc = initial_pc + expectedOffset;
		if (currentPc != expected_pc) {
			throw std::runtime_error("PC assertion failed: Expected " +
									 std::to_string(expected_pc) +
									 ", got " + std::to_string(currentPc) +
									 ", Error: " +
									 std::to_string(currentPc - expected_pc));
		}
	}
};

#endif //AYB_INSTR_DEBUG_H
