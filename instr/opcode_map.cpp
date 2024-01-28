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

	/* Load Instructions 8-bit */
	instructions[0x06] = [this]() { this->ld_r8_n8(B); };
	instructions[0x16] = [this]() { this->ld_r8_n8(D); };
	instructions[0x26] = [this]() { this->ld_r8_n8(H); };
	instructions[0x0E] = [this]() { this->ld_r8_n8(C); };
	instructions[0x1E] = [this]() { this->ld_r8_n8(E); };
	instructions[0x2E] = [this]() { this->ld_r8_n8(L); };
	instructions[0x3E] = [this]() { this->ld_r8_n8(A); };

	instructions[0x40] = [this]() { this->ld_r8_r8(B, B); };
	instructions[0x41] = [this]() { this->ld_r8_r8(B, C); };
	instructions[0x42] = [this]() { this->ld_r8_r8(B, D); };
	instructions[0x43] = [this]() { this->ld_r8_r8(B, E); };
	instructions[0x44] = [this]() { this->ld_r8_r8(B, H); };
	instructions[0x45] = [this]() { this->ld_r8_r8(B, L); };
	instructions[0x47] = [this]() { this->ld_r8_r8(B, A); };
	instructions[0x48] = [this]() { this->ld_r8_r8(C, B); };
	instructions[0x49] = [this]() { this->ld_r8_r8(C, C); };
	instructions[0x4A] = [this]() { this->ld_r8_r8(C, D); };
	instructions[0x4B] = [this]() { this->ld_r8_r8(C, E); };
	instructions[0x4C] = [this]() { this->ld_r8_r8(C, H); };
	instructions[0x4D] = [this]() { this->ld_r8_r8(C, L); };
	instructions[0x4F] = [this]() { this->ld_r8_r8(C, A); };

	instructions[0x50] = [this]() { this->ld_r8_r8(D, B); };
	instructions[0x51] = [this]() { this->ld_r8_r8(D, C); };
	instructions[0x52] = [this]() { this->ld_r8_r8(D, D); };
	instructions[0x53] = [this]() { this->ld_r8_r8(D, E); };
	instructions[0x54] = [this]() { this->ld_r8_r8(D, H); };
	instructions[0x55] = [this]() { this->ld_r8_r8(D, L); };
	instructions[0x57] = [this]() { this->ld_r8_r8(D, A); };
	instructions[0x58] = [this]() { this->ld_r8_r8(E, B); };
	instructions[0x59] = [this]() { this->ld_r8_r8(E, C); };
	instructions[0x5A] = [this]() { this->ld_r8_r8(E, D); };
	instructions[0x5B] = [this]() { this->ld_r8_r8(E, E); };
	instructions[0x5C] = [this]() { this->ld_r8_r8(E, H); };
	instructions[0x5D] = [this]() { this->ld_r8_r8(E, L); };
	instructions[0x5F] = [this]() { this->ld_r8_r8(E, A); };

	instructions[0x60] = [this]() { this->ld_r8_r8(H, B); };
	instructions[0x61] = [this]() { this->ld_r8_r8(H, C); };
	instructions[0x62] = [this]() { this->ld_r8_r8(H, D); };
	instructions[0x63] = [this]() { this->ld_r8_r8(H, E); };
	instructions[0x64] = [this]() { this->ld_r8_r8(H, H); };
	instructions[0x65] = [this]() { this->ld_r8_r8(H, L); };
	instructions[0x67] = [this]() { this->ld_r8_r8(H, A); };
	instructions[0x68] = [this]() { this->ld_r8_r8(L, B); };
	instructions[0x69] = [this]() { this->ld_r8_r8(L, C); };
	instructions[0x6A] = [this]() { this->ld_r8_r8(L, D); };
	instructions[0x6B] = [this]() { this->ld_r8_r8(L, E); };
	instructions[0x6C] = [this]() { this->ld_r8_r8(L, H); };
	instructions[0x6D] = [this]() { this->ld_r8_r8(L, L); };
	instructions[0x6F] = [this]() { this->ld_r8_r8(L, A); };

	instructions[0x78] = [this]() { this->ld_r8_r8(A, B); };
	instructions[0x79] = [this]() { this->ld_r8_r8(A, C); };
	instructions[0x7A] = [this]() { this->ld_r8_r8(A, D); };
	instructions[0x7B] = [this]() { this->ld_r8_r8(A, E); };
	instructions[0x7C] = [this]() { this->ld_r8_r8(A, H); };
	instructions[0x7D] = [this]() { this->ld_r8_r8(A, L); };
	instructions[0x7F] = [this]() { this->ld_r8_r8(A, A); };

	/* Arithmetic 16-bit Instructions */
	instructions[0x09] = [this]() { this->add_hl_r16(BC); };
	instructions[0x19] = [this]() { this->add_hl_r16(DE); };
	instructions[0x29] = [this]() { this->add_hl_r16(HL); };
	instructions[0x39] = [this]() { this->add_hl_r16(SP); };

	instructions[0x03] = [this]() { this->inc_r16(BC); };
	instructions[0x13] = [this]() { this->inc_r16(DE); };
	instructions[0x23] = [this]() { this->inc_r16(HL); };
	instructions[0x33] = [this]() { this->inc_r16(SP); };
	instructions[0x0B] = [this]() { this->dec_r16(BC); };
	instructions[0x1B] = [this]() { this->dec_r16(DE); };
	instructions[0x2B] = [this]() { this->dec_r16(HL); };
	instructions[0x3B] = [this]() { this->dec_r16(SP); };

	instructions[0xE8] = [this]() { this->add_sp_e8(); };

	/* Arithmetic 8-bit Instructions */
	instructions[0x04] = [this]() { this->inc_r8(B); };
	instructions[0x14] = [this]() { this->inc_r8(D); };
	instructions[0x24] = [this]() { this->inc_r8(H); };
	instructions[0x34] = [this]() { this->inc_hl(); };
	instructions[0x0C] = [this]() { this->inc_r8(C); };
	instructions[0x1C] = [this]() { this->inc_r8(E); };
	instructions[0x2C] = [this]() { this->inc_r8(L); };
	instructions[0x3C] = [this]() { this->inc_r8(A); };

	instructions[0x05] = [this]() { this->dec_r8(B); };
	instructions[0x15] = [this]() { this->dec_r8(D); };
	instructions[0x25] = [this]() { this->dec_r8(H); };
	instructions[0x35] = [this]() { this->dec_hl(); };
	instructions[0x0D] = [this]() { this->dec_r8(C); };
	instructions[0x1D] = [this]() { this->dec_r8(E); };
	instructions[0x2D] = [this]() { this->dec_r8(L); };
	instructions[0x3D] = [this]() { this->dec_r8(A); };

	instructions[0x80] = [this]() { this->add_a_r8(B); };
	instructions[0x81] = [this]() { this->add_a_r8(C); };
	instructions[0x82] = [this]() { this->add_a_r8(D); };
	instructions[0x83] = [this]() { this->add_a_r8(E); };
	instructions[0x84] = [this]() { this->add_a_r8(H); };
	instructions[0x85] = [this]() { this->add_a_r8(L); };
	instructions[0x86] = [this]() { this->add_a_hl(); };
	instructions[0x87] = [this]() { this->add_a_r8(A); };

	instructions[0x88] = [this]() { this->adc_a_r8(B); };
	instructions[0x89] = [this]() { this->adc_a_r8(C); };
	instructions[0x8A] = [this]() { this->adc_a_r8(D); };
	instructions[0x8B] = [this]() { this->adc_a_r8(E); };
	instructions[0x8C] = [this]() { this->adc_a_r8(H); };
	instructions[0x8D] = [this]() { this->adc_a_r8(L); };
	instructions[0x8E] = [this]() { this->adc_a_hl(); };
	instructions[0x8F] = [this]() { this->adc_a_r8(A); };

	instructions[0x90] = [this]() { this->sub_a_r8(B); };
	instructions[0x91] = [this]() { this->sub_a_r8(C); };
	instructions[0x92] = [this]() { this->sub_a_r8(D); };
	instructions[0x93] = [this]() { this->sub_a_r8(E); };
	instructions[0x94] = [this]() { this->sub_a_r8(H); };
	instructions[0x95] = [this]() { this->sub_a_r8(L); };
	instructions[0x96] = [this]() { this->sub_a_hl(); };
	instructions[0x97] = [this]() { this->sub_a_r8(A); };

	instructions[0x98] = [this]() { this->sbc_a_r8(B); };
	instructions[0x99] = [this]() { this->sbc_a_r8(C); };
	instructions[0x9A] = [this]() { this->sbc_a_r8(D); };
	instructions[0x9B] = [this]() { this->sbc_a_r8(E); };
	instructions[0x9C] = [this]() { this->sbc_a_r8(H); };
	instructions[0x9D] = [this]() { this->sbc_a_r8(L); };
	instructions[0x9E] = [this]() { this->sbc_a_hl(); };
	instructions[0x9F] = [this]() { this->sbc_a_r8(A); };

	instructions[0xA0] = [this]() { this->and_a_r8(B); };
	instructions[0xA1] = [this]() { this->and_a_r8(C); };
	instructions[0xA2] = [this]() { this->and_a_r8(D); };
	instructions[0xA3] = [this]() { this->and_a_r8(E); };
	instructions[0xA4] = [this]() { this->and_a_r8(H); };
	instructions[0xA5] = [this]() { this->and_a_r8(L); };
	instructions[0xA6] = [this]() { this->and_a_hl(); };
	instructions[0xA7] = [this]() { this->and_a_r8(A); };

	instructions[0xA8] = [this]() { this->xor_a_r8(B); };
	instructions[0xA9] = [this]() { this->xor_a_r8(C); };
	instructions[0xAA] = [this]() { this->xor_a_r8(D); };
	instructions[0xAB] = [this]() { this->xor_a_r8(E); };
	instructions[0xAC] = [this]() { this->xor_a_r8(H); };
	instructions[0xAD] = [this]() { this->xor_a_r8(L); };
	instructions[0xAE] = [this]() { this->xor_a_hl(); };
	instructions[0xAF] = [this]() { this->xor_a_r8(A); };

	instructions[0xB0] = [this]() { this->or_a_r8(B); };
	instructions[0xB1] = [this]() { this->or_a_r8(C); };
	instructions[0xB2] = [this]() { this->or_a_r8(D); };
	instructions[0xB3] = [this]() { this->or_a_r8(E); };
	instructions[0xB4] = [this]() { this->or_a_r8(H); };
	instructions[0xB5] = [this]() { this->or_a_r8(L); };
	instructions[0xB6] = [this]() { this->or_a_hl(); };
	instructions[0xB7] = [this]() { this->or_a_r8(A); };

	instructions[0xB8] = [this]() { this->cp_a_r8(B); };
	instructions[0xB9] = [this]() { this->cp_a_r8(C); };
	instructions[0xBA] = [this]() { this->cp_a_r8(D); };
	instructions[0xBB] = [this]() { this->cp_a_r8(E); };
	instructions[0xBC] = [this]() { this->cp_a_r8(H); };
	instructions[0xBD] = [this]() { this->cp_a_r8(L); };
	instructions[0xBE] = [this]() { this->cp_a_hl(); };
	instructions[0xBF] = [this]() { this->cp_a_r8(A); };

	instructions[0xC6] = [this]() { this->add_a_n8(); };
	instructions[0xD6] = [this]() { this->sub_a_n8(); };
	instructions[0xE6] = [this]() { this->and_a_n8(); };
	instructions[0xF6] = [this]() { this->or_a_n8(); };
	instructions[0xCE] = [this]() { this->adc_a_n8(); };
	instructions[0xDE] = [this]() { this->sbc_a_n8(); };
	instructions[0xEE] = [this]() { this->xor_a_n8(); };
	instructions[0xFE] = [this]() { this->cp_a_n8(); };

	instructions[0x27] = [this]() { this->daa(); };
	instructions[0x37] = [this]() { this->scf(); };
	instructions[0x2F] = [this]() { this->cpl(); };
	instructions[0x3F] = [this]() { this->ccf(); };
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