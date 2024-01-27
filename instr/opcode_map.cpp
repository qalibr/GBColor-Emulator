#include "operator.h"

void OpcodeMap::init_instructions() {
	/* Control Instructions */
	instructions[0xCB] = [this]() { this->prefix_cb(); };
	instructions[0x00] = [this]() { this->nop(); };
	instructions[0x10] = [this]() { this->stop(); };
	instructions[0x76] = [this]() { this->halt(); };
	instructions[0xF3] = [this]() { this->di(); };
	instructions[0xFB] = [this]() { this->ei(); };

	/* Rotate/Shift Instructions - non-prefixed */
	instructions[0x07] = [this]() { this->rlca(); };
	instructions[0x17] = [this]() { this->rla(); };
	instructions[0x0F] = [this]() { this->rrca(); };
	instructions[0x1F] = [this]() { this->rra(); };

	/* Jump/Call Instructions */
	instructions[0xE9] = [this]() { this->jp_hl(); };
	instructions[0xD9] = [this]() { this->reti(); };
	instructions[0x18] = [this]() { this->jr_e8(); };
	instructions[0x20] = [this]() { this->jr_cc_e8(nz); };
	instructions[0x28] = [this]() { this->jr_cc_e8(z); };
	instructions[0x30] = [this]() { this->jr_cc_e8(nc); };
	instructions[0x38] = [this]() { this->jr_cc_e8(c); };
}
void OpcodeMap::execute(uint8_t op_code) {
	if (instructions[op_code]) {
		instructions[op_code]();

		/*
		 * If EI has been called, interrupts are pending, and IME is false,
		 * we set another flag to delay IME being set until after the next
		 * instruction has been executed.
		 */
		if (cpu.get_cpu_flag().get_enable_interrupts_pending()
			&& !cpu.get_cpu_flag().get_ime()) {
			cpu.get_cpu_flag().set_enable_interrupts(true);
		}

		/* Set IME and clear the EI flags */
		if (cpu.get_cpu_flag().get_enable_interrupts_pending()
			&& cpu.get_cpu_flag().get_enable_interrupts()) {
			cpu.get_cpu_flag().set_enable_interrupts_pending(false);
			cpu.get_cpu_flag().set_enable_interrupts(false);
			cpu.get_cpu_flag().set_ime(true);
		}

		return;
	}

	throw std::runtime_error("Invalid opcode");
}