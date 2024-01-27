#ifndef AYB_OPERATOR_H
#define AYB_OPERATOR_H

#include "functional"
#include "cstdint"

#include "cpu.h"
#include "mem_util.h"
#include "timer.h"
#include "hw_reg.h"

enum Reg {
	A,
	F,
	B,
	C,
	D,
	E,
	H,
	L,
	AF,
	BC,
	DE,
	HL,
	SP,
	PC,
	n8,
};

enum Flags {
	Z,
	N,
	HC,
	CY,
	IME,
	STOP,
	HALT
};

enum Cc {
	nz,
	z,
	nc,
	c
};

enum Rst {
	rst_00h = 0x00,
	rst_08h = 0x08,
	rst_10h = 0x10,
	rst_18h = 0x18,
	rst_20h = 0x20,
	rst_28h = 0x28,
	rst_30h = 0x30,
	rst_38h = 0x38
};

class Operator {
protected:
	Cpu               cpu;
	MemoryUtility     mem_util;
	Timer             timer;
	HardwareRegisters hw_reg;

public:
	Operator();
	~Operator();

	uint16_t get_reg(Reg reg);
	void set_reg(Reg reg, uint16_t val);
	uint8_t get_flag(Flags flag);
	void set_flag(Flags flag, uint8_t val);


	uint8_t fetch_byte();         // n8 | Fetches a byte from memory and increments PC
	int8_t fetch_signed_byte();   // e8 | Fetches a signed byte from memory and increments PC
	uint16_t fetch_a8_address();  // a8 Fetches an address from memory and increments PC
	uint16_t fetch_a16_address(); // a16/n16 | Fetches a word from memory and increments PC twice.
};

class Instructions : public Operator {
protected:
	void cost(uint8_t size, uint8_t cycles);
	void half_carry_on_add(uint8_t val1, uint8_t val2);
	void half_carry_on_add(uint16_t val1, uint16_t val2);
	void half_carry_on_sub(uint8_t val1, uint8_t val2);
	void half_carry_on_sub(uint16_t val1, uint16_t val2);

	/*
	 * https://gbdev.io/gb-opcodes/optables/ - Opcode table
	 * - n8 means immediate 8-bit data
	 * - n16 means immediate little-endian 16-bit data
	 * - a8 means 8-bit unsigned data, which is added to $FF00 in certain instructions to create
	 * 	 a 16-bit address in HRAM (High RAM).
	 * 	 I can see two instructions that use this:
	 * 	 LDH (a8), A at 0xE0
	 * 	 LDH A, (a8) at 0xF0
	 * - a16 means little-endian 16-bit address
	 * - e8 means 8-bit signed data
	 */

	/* Control Instructions */
	void nop();                         // - - - - | 1, 4 | 0x00
	void stop();                        // - - - - | 1, 4 | 0x10
	void halt();                        // - - - - | 1, 4 | 0x76
	void di();                          // - - - - | 1, 4 | 0xF3
	void ei();                          // - - - - | 1, 4 | 0xFB

	/* Rotate/Shift Instructions - non-prefixed */
	void rlca();                        // 0 0 0 C | 1, 4 | 0x07
	void rla();                         // 0 0 0 C | 1, 4 | 0x17
	void rrca();                        // 0 0 0 C | 1, 4 | 0x0F
	void rra();                         // 0 0 0 C | 1, 4 | 0x1F

	/* Jump/Call Instructions */
	void jp_hl();                       // - - - - | 1, 4     | 0xE9
	void reti();                        // - - - - | 1, 16    | 0xD9
	void jr_cc_e8(Cc cc);               // - - - - | 2, 12/8  | 0x20, 0x28, 0x30, 0x38
	void jr_e8();                       // - - - - | 2, 12    | 0x18
	void jp_cc_a16(Cc cc);              // - - - - | 3, 16/12 | 0xC2, 0xCA, 0xD2, 0xDA
	void jp_a16();                      // - - - - | 3, 16    | 0xC3
	void call_cc_a16(Cc cc);            // - - - - | 3, 24/12 | 0xC4, 0xCC, 0xD4, 0xDC
	void call_a16();                    // - - - - | 3, 24    | 0xCD
	void ret_cc(Cc cc);                 // - - - - | 1, 20/8  | 0xC0, 0xC8, 0xD0, 0xD8
	void ret();                         // - - - - | 1, 16    | 0xC9
	void rst(Rst rst);                  // - - - - | 1, 16    | 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF

	/* Load Instruction 16-bit */
	void ld_r16_n16(Reg reg);           // - - - - | 3, 12 | 0x01, 0x11, 0x21, 0x31
	void ld_indirect_a16_sp();          // - - - - | 3, 20 | 0x08
	void ld_pop_r16(Reg reg);           // - - - - | 1, 12 | 0xC1, 0xD1, 0xE1, 0xF1
	void ld_push_r16(Reg reg);          // - - - - | 1, 16 | 0xC5, 0xD5, 0xE5, 0xF5
	void ld_hl_sp_e8();                 // 0 0 H C | 2, 12 | 0xF8
	void ld_sp_hl();                    // - - - - | 1, 8  | 0xF9

	/* Load Instructions 8-bit - Indirect Addressing */
	void ld_indirect_bc_a();            // - - - - | 1, 8  | 0x02
	void ld_indirect_de_a();            // - - - - | 1, 8  | 0x12
	void ld_indirect_hli_a();           // - - - - | 1, 8  | 0x22
	void ld_indirect_hld_a();           // - - - - | 1, 8  | 0x32
	void ld_indirect_a_bc();            // - - - - | 1, 8  | 0x0A
	void ld_indirect_a_de();            // - - - - | 1, 8  | 0x1A
	void ld_indirect_a_hli();           // - - - - | 1, 8  | 0x2A
	void ld_indirect_a_hld();           // - - - - | 1, 8  | 0x3A
	void ld_indirect_hl_n8();           // - - - - | 2, 12 | 0x36
	void ld_r8_indirect_hl(Reg reg);    // - - - - | 1, 8  | 0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E
	void ld_hl_indirect_r8(Reg reg);    // - - - - | 1, 8  | 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77
	void ldh_indirect_a8_a();           // - - - - | 2, 12 | 0xE0
	void ldh_indirect_a_a8();           // - - - - | 2, 12 | 0xF0
	void ld_indirect_c_a();             // - - - - | 1, 8  | 0xE2
	void ld_indirect_a_c();             // - - - - | 1, 8  | 0xF2
	void ld_indirect_a16_a();           // - - - - | 3, 16 | 0xEA
	void ld_indirect_a_a16();           // - - - - | 3, 16 | 0xFA

	/* Load Instructions 8-bit */
	void ld_r8_n8(Reg reg, Reg n8);     // - - - - | 2, 8 | 0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x3E

	/*
	 * B: 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47
	 * C: 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F
	 * D: 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57
	 * E: 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5F
	 * H: 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x67
	 * L: 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6F
	 * A: 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F
	 */
	void ld_r8_r8(Reg reg1, Reg reg2);  // - - - - | 1, 4 |

	/* Arithmetic 16-bit Instructions */
	void add_hl_r16(Reg reg);           // - 0 H C | 1, 8 | 0x09, 0x19, 0x29, 0x39
	void add_sp_e8();                   // 0 0 H C | 2, 16 | 0xE8
	void dec_r16(Reg reg);              // - - - - | 1, 8 | 0x0B, 0x1B, 0x2B, 0x3B
	void inc_r16(Reg reg);              // - - - - | 1, 8 | 0x03, 0x13, 0x23, 0x33

	/* Arithmetic 8-bit Instructions */
	void inc_r8(Reg reg);               // Z 0 H - | 1, 4 | 0x04, 0x14, 0x24, 0x0C, 0x1C, 0x2C, 0x3C
	void inc_hl();                      // Z 0 H - | 1, 12 | 0x34
	void dec_r8(Reg reg);               // Z 1 H - | 1, 4 | 0x05, 0x15, 0x25, 0x0D, 0x1D, 0x2D, 0x3D
	void dec_hl();                      // Z 1 H - | 1, 12 | 0x35
	void add_a_r8(Reg reg);             // Z 0 H C | 1, 4 | 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87
	void add_a_hl();                    // Z 0 H C | 1, 8 | 0x86
	void add_a_n8();                    // Z 0 H C | 2, 8 | 0xC6
	void adc_a_r8(Reg reg);             // Z 0 H C | 1, 4 | 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F
	void adc_a_hl();                    // Z 0 H C | 1, 8 | 0x8E
	void adc_a_n8();                    // Z 0 H C | 2, 8 | 0xCE
	void sub_a_r8(Reg reg);             // Z 1 H C | 1, 4 | 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97
	void sub_a_hl();                    // Z 1 H C | 1, 8 | 0x96
	void sub_a_n8();                    // Z 1 H C | 2, 8 | 0xD6
	void sbc_a_r8(Reg reg);             // Z 1 H C | 1, 4 | 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9F
	void sbc_a_hl();                    // Z 1 H C | 1, 8 | 0x9E
	void sbc_a_n8();                    // Z 1 H C | 2, 8 | 0xDE
	void and_a_r8(Reg reg);             // Z 0 1 0 | 1, 4 | 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7
	void and_a_hl();                    // Z 0 1 0 | 1, 8 | 0xA6
	void and_a_n8();                    // Z 0 1 0 | 2, 8 | 0xE6
	void xor_a_r8(Reg reg);             // Z 0 0 0 | 1, 4 | 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAF
	void xor_a_hl();                    // Z 0 0 0 | 1, 8 | 0xAE
	void xor_a_n8();                    // Z 0 0 0 | 2, 8 | 0xEE
	void or_a_r8(Reg reg);              // Z 0 0 0 | 1, 4 | 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7
	void or_a_hl();                     // Z 0 0 0 | 1, 8 | 0xB6
	void or_a_n8();                     // Z 0 0 0 | 2, 8 | 0xF6
	void cp_a_r8(Reg reg);              // Z 1 H C | 1, 4 | 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBF
	void cp_a_hl();                     // Z 1 H C | 1, 8 | 0xBE
	void cp_a_n8();                     // Z 1 H C | 2, 8 | 0xFE
	void daa();                         // Z - 0 C | 1, 4 | 0x27
	void cpl();                         // - 1 1 - | 1, 4 | 0x2F
	void scf();                         // - 0 0 1 | 1, 4 | 0x37
	void ccf();                         // - 0 0 C | 1, 4 | 0x3F

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *///  * * * * * * * * * * * * * * * * * * * * * * * * * * *
	/* 				     Prefix Instructions				   */
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *///  * * * * * * * * * * * * * * * * * * * * * * * * * * *
	void rlc_r8(Reg reg);               // Z 0 0 C | 2, 8  | 0xCB: 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07
	void rlc_hl();                      // Z 0 0 C | 2, 16 | 0xCB: 0x06
	void rl_r8(Reg reg);                // Z 0 0 C | 2, 8  | 0xCB: 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x17
	void rl_hl();                       // Z 0 0 C | 2, 16 | 0xCB: 0x16
	void rrc_r8(Reg reg);               // Z 0 0 C | 2, 8  | 0xCB: 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0F
	void rrc_hl();                      // Z 0 0 C | 2, 16 | 0xCB: 0x0E
	void rr_r8(Reg reg);                // Z 0 0 C | 2, 8  | 0xCB: 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F
	void rr_hl();                       // Z 0 0 C | 2, 16 | 0xCB: 0x1E
	void sla_r8(Reg reg);               // Z 0 0 C | 2, 8  | 0xCB: 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x27
	void sla_hl();                      // Z 0 0 C | 2, 16 | 0xCB: 0x26
	void sra_r8(Reg reg);               // Z 0 0 0 | 2, 8  | 0xCB: 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2F
	void sra_hl();                      // Z 0 0 0 | 2, 16 | 0xCB: 0x2E
	void swap_r8(Reg reg);              // Z 0 0 0 | 2, 8  | 0xCB: 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x37
	void swap_hl();                     // Z 0 0 0 | 2, 16 | 0xCB: 0x36
	void srl_r8(Reg reg);               // Z 0 0 C | 2, 8  | 0xCB: 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3F
	void srl_hl();                      // Z 0 0 C | 2, 16 | 0xCB: 0x3E

	/* CB:
	 * 0: 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47
	 * 1: 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F
	 * 2: 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57
	 * 3: 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5F
	 * 4: 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x67
	 * 5: 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6F
	 * 6: 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77
	 * 7: 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F
	 */
	void bit_r8(Reg reg, uint8_t bit);  // Z 0 1 - | 2, 8

	/* CB:
	 * 0: 0x46
	 * 1: 0x4E
	 * 2: 0x56
	 * 3: 0x5E
	 * 4: 0x66
	 * 5: 0x6E
	 * 6: 0x76
	 * 7: 0x7E
	 */
	void bit_hl(uint8_t bit);           // Z 0 1 - | 2, 16

	/* CB:
	 * 0: 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87
	 * 1: 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F
	 * 2: 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97
	 * 3: 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9F
	 * 4: 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7
	 * 5: 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAF
	 * 6: 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7
	 * 7: 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBF
	 */
	void res_r8(Reg reg, uint8_t bit);  // - - - - | 2, 8

	/* CB:
	 * 0: 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC7
	 * 1: 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCF
	 * 2: 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD7
	 * 3: 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDF
	 * 4: 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE7
	 * 5: 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEF
	 * 6: 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF7
	 * 7: 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFF
	 */
	void set_r8(Reg reg, uint8_t bit);  // - - - - | 2, 8

public:
	Instructions() = default;
	~Instructions() = default;
};

class OpcodeMap : public Instructions {
private:
	using Instruction = std::function<void()>;
	Instruction instructions[256]    = {};
	Instruction cb_instructions[256] = {};
	void prefix_cb(); // - - - - | 1, 4 | 0xCB

public:
	OpcodeMap() {
		init_instructions();
	};
	~OpcodeMap();

	Instruction* get_cb_instruction() {
		return cb_instructions;
	}
	void init_instructions();
	void execute(uint8_t op_code);
};

#endif //AYB_OPERATOR_H
