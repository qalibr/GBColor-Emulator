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

	instructions[0x20] = [this]() { this->jr_cc_e8(nz); };
	instructions[0x28] = [this]() { this->jr_cc_e8(z); };
	instructions[0x30] = [this]() { this->jr_cc_e8(nc); };
	instructions[0x38] = [this]() { this->jr_cc_e8(c); };
	instructions[0x18] = [this]() { this->jr_e8(); };

	instructions[0xC2] = [this]() { this->jp_cc_a16(nz); };
	instructions[0xCA] = [this]() { this->jp_cc_a16(z); };
	instructions[0xD2] = [this]() { this->jp_cc_a16(nc); };
	instructions[0xDA] = [this]() { this->jp_cc_a16(c); };
	instructions[0xC3] = [this]() { this->jp_a16(); };

	instructions[0xCD] = [this]() { this->call_a16(); };
	instructions[0xC4] = [this]() { this->call_cc_a16(nz); };
	instructions[0xCC] = [this]() { this->call_cc_a16(z); };
	instructions[0xD4] = [this]() { this->call_cc_a16(nc); };
	instructions[0xDC] = [this]() { this->call_cc_a16(c); };

	instructions[0xC0] = [this]() { this->ret_cc(nz); };
	instructions[0xC8] = [this]() { this->ret_cc(z); };
	instructions[0xD0] = [this]() { this->ret_cc(nc); };
	instructions[0xD8] = [this]() { this->ret_cc(c); };
	instructions[0xC9] = [this]() { this->ret(); };

	instructions[0xC7] = [this]() { this->rst(rst_00h); };
	instructions[0xCF] = [this]() { this->rst(rst_08h); };
	instructions[0xD7] = [this]() { this->rst(rst_10h); };
	instructions[0xDF] = [this]() { this->rst(rst_18h); };
	instructions[0xE7] = [this]() { this->rst(rst_20h); };
	instructions[0xEF] = [this]() { this->rst(rst_28h); };
	instructions[0xF7] = [this]() { this->rst(rst_30h); };
	instructions[0xFF] = [this]() { this->rst(rst_38h); };

	/* Load Instruction 16-bit */
	instructions[0x01] = [this]() { this->ld_r16_n16(BC); };
	instructions[0x11] = [this]() { this->ld_r16_n16(DE); };
	instructions[0x21] = [this]() { this->ld_r16_n16(HL); };
	instructions[0x31] = [this]() { this->ld_r16_n16(SP); };
	instructions[0x08] = [this]() { this->ld_a16_sp(); };

	instructions[0xC1] = [this]() { this->ld_pop_r16(BC); };
	instructions[0xD1] = [this]() { this->ld_pop_r16(DE); };
	instructions[0xE1] = [this]() { this->ld_pop_r16(HL); };
	instructions[0xF1] = [this]() { this->ld_pop_r16(AF); };
	instructions[0xC5] = [this]() { this->ld_push_r16(BC); };
	instructions[0xD5] = [this]() { this->ld_push_r16(DE); };
	instructions[0xE5] = [this]() { this->ld_push_r16(HL); };
	instructions[0xF5] = [this]() { this->ld_push_r16(AF); };
	instructions[0xF8] = [this]() { this->ld_hl_sp_e8(); };
	instructions[0xF9] = [this]() { this->ld_sp_hl(); };

	/* Load Instructions 8-bit - Indirect Addressing */
	instructions[0x02] = [this]() { this->ld_bc_a(); };
	instructions[0x12] = [this]() { this->ld_de_a(); };
	instructions[0x0A] = [this]() { this->ld_a_bc(); };
	instructions[0x1A] = [this]() { this->ld_a_de(); };
	instructions[0xE2] = [this]() { this->ld_c_a(); };
	instructions[0xF2] = [this]() { this->ld_a_c(); };
	instructions[0x46] = [this]() { this->ld_r8_hl(B); };
	instructions[0x4E] = [this]() { this->ld_r8_hl(C); };
	instructions[0x56] = [this]() { this->ld_r8_hl(D); };
	instructions[0x5E] = [this]() { this->ld_r8_hl(E); };
	instructions[0x66] = [this]() { this->ld_r8_hl(H); };
	instructions[0x6E] = [this]() { this->ld_r8_hl(L); };
	instructions[0x7E] = [this]() { this->ld_hl_r8(A); };
	instructions[0x70] = [this]() { this->ld_hl_r8(B); };
	instructions[0x71] = [this]() { this->ld_hl_r8(C); };
	instructions[0x72] = [this]() { this->ld_hl_r8(D); };
	instructions[0x73] = [this]() { this->ld_hl_r8(E); };
	instructions[0x74] = [this]() { this->ld_hl_r8(H); };
	instructions[0x75] = [this]() { this->ld_hl_r8(L); };
	instructions[0x77] = [this]() { this->ld_hl_r8(A); };
	instructions[0x22] = [this]() { this->ld_hli_a(); };
	instructions[0x32] = [this]() { this->ld_hld_a(); };
	instructions[0x2A] = [this]() { this->ld_a_hli(); };
	instructions[0x3A] = [this]() { this->ld_a_hld(); };
	instructions[0x36] = [this]() { this->ld_hl_n8(); };
	instructions[0xE0] = [this]() { this->ldh_a8_a(); };
	instructions[0xF0] = [this]() { this->ldh_a_a8(); };
	instructions[0xEA] = [this]() { this->ld_a16_a(); };
	instructions[0xFA] = [this]() { this->ld_a_a16(); };
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