#ifndef AYB_OPERATOR_H
#define AYB_OPERATOR_H

#include "functional"
#include "cstdint"

#include "cpu.h"

enum Registers {
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
	PC
};

enum Flags {
	Z,
	N,
	HC,
	CY
};

class Operator {
protected:
	Cpu cpu;

public:
	Operator();
	~Operator();

	uint16_t get_reg(Registers reg);
	void set_reg(Registers reg, uint16_t val);
	uint8_t get_flag(Flags flag);
	void set_flag(Flags flag, uint8_t val);

	uint8_t fetch_byte();         // n8 | Fetches a byte from memory and increments PC
	int8_t fetch_signed_byte();   // e8 | Fetches a signed byte from memory and increments PC
	uint16_t fetch_a8_address();  // a8 Fetches an address from memory and increments PC
	uint16_t fetch_word();        // a16/n16 | Fetches a word from memory and increments PC twice.
};

class Instructions : public Operator {
protected:
	void cost(uint8_t cycles, uint8_t size);
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
	void nop();                     // - - - - | 1, 4 | 0x00
	void stop();                    // - - - - | 1, 4 | 0x10
	void halt();                    // - - - - | 1, 4 | 0x76
	void prefix_cb();               // - - - - | 1, 4 | 0xCB
	void di();                      // - - - - | 1, 4 | 0xF3
	void ei();                      // - - - - | 1, 4 | 0xFB

	// TODO: Generalize these using an enum
	// Pay attention to e8, n8, a16 and so on, those meanings are very important.
	/* Jump/Call Instructions */
	void jr_e8();                   // - - - - | 2, 12 | 0x18
	void jr_nz_e8();                // - - - - | 2, 12/8 | 0x20
	void jr_z_e8();                 // - - - - | 2, 12/8 | 0x28
	void jr_nc_e8();                // - - - - | 2, 12/8 | 0x30
	void jr_c_e8();                 // - - - - | 2, 12/8 | 0x38
	void jp_nz_a16();               // - - - - | 3, 16/12 | 0xC2
	void jp_a16();                  // - - - - | 3, 16 | 0xC3
	void jp_z_a16();                // - - - - | 3, 16/12 | 0xCA
	void jp_nc_a16();               // - - - - | 3, 16/12 | 0xD2
	void jp_c_a16();                // - - - - | 3, 16/12 | 0xDA
	void jp_hl();                   // - - - - | 1, 4 | 0xE9
	void call_nz_a16();             // - - - - | 3, 24/12 | 0xC4
	void call_z_a16();              // - - - - | 3, 24/12 | 0xCC
	void call_a16();                // - - - - | 3, 24 | 0xCD
	void call_nc_a16();             // - - - - | 3, 24/12 | 0xD4
	void call_c_a16();              // - - - - | 3, 24/12 | 0xDC
	void ret_nz();                  // - - - - | 1, 20/8 | 0xC0
	void ret_z();                   // - - - - | 1, 20/8 | 0xC8
	void ret();                     // - - - - | 1, 16 | 0xC9
	void ret_nc();                  // - - - - | 1, 20/8 | 0xD0
	void ret_c();                   // - - - - | 1, 20/8 | 0xD8
	void reti();                    // - - - - | 1, 16 | 0xD9
	void rst_00h();                 // - - - - | 1, 16 | 0xC7
	void rst_08h();                 // - - - - | 1, 16 | 0xCF
	void rst_10h();                 // - - - - | 1, 16 | 0xD7
	void rst_18h();                 // - - - - | 1, 16 | 0xDF
	void rst_20h();                 // - - - - | 1, 16 | 0xE7
	void rst_28h();                 // - - - - | 1, 16 | 0xEF
	void rst_30h();                 // - - - - | 1, 16 | 0xF7
	void rst_38h();                 // - - - - | 1, 16 | 0xFF

	/* Load Instructions - Indirect Addressing */
	void ld_indirect_bc_a();        // - - - - | 1, 8 | 0x02
	void ld_indirect_de_a();        // - - - - | 1, 8 | 0x12
	void ld_indirect_hli_a();       // - - - - | 1, 8 | 0x22
	void ld_indirect_hld_a();       // - - - - | 1, 8 | 0x32
	void ld_indirect_a_bc();        // - - - - | 1, 8 | 0x0A
	void ld_indirect_a_de();        // - - - - | 1, 8 | 0x1A
	void ld_indirect_a_hli();       // - - - - | 1, 8 | 0x2A
	void ld_indirect_a_hld();       // - - - - | 1, 8 | 0x3A
	void ld_indirect_hl_n8();       // - - - - | 2, 12 | 0x36
	void ld_b_hl();                 // - - - - | 1, 8 | 0x46
	void ld_c_hl();                 // - - - - | 1, 8 | 0x4E

	/* Load Instructions */
	void ld_b_n8();                 // - - - - | 2, 8 | 0x06
	void ld_c_n8();                 // - - - - | 2, 8 | 0x0E
	void ld_d_n8();                 // - - - - | 2, 8 | 0x16
	void ld_e_n8();                 // - - - - | 2, 8 | 0x1E
	void ld_h_n8();                 // - - - - | 2, 8 | 0x26
	void ld_l_n8();                 // - - - - | 2, 8 | 0x2E
	void ld_a_n8();                 // - - - - | 2, 8 | 0x3E
	void ld_b_b();                  // - - - - | 1, 4 | 0x40
	void ld_b_c();                  // - - - - | 1, 4 | 0x41
	void ld_b_d();                  // - - - - | 1, 4 | 0x42
	void ld_b_e();                  // - - - - | 1, 4 | 0x43
	void ld_b_h();                  // - - - - | 1, 4 | 0x44
	void ld_b_l();                  // - - - - | 1, 4 | 0x45
	void ld_b_a();                  // - - - - | 1, 4 | 0x47
	void ld_c_b();                  // - - - - | 1, 4 | 0x48
	void ld_c_c();                  // - - - - | 1, 4 | 0x49
	void ld_c_d();                  // - - - - | 1, 4 | 0x4A
	void ld_c_e();                  // - - - - | 1, 4 | 0x4B
	void ld_c_h();                  // - - - - | 1, 4 | 0x4C
	void ld_c_l();                  // - - - - | 1, 4 | 0x4D
	void ld_c_a();                  // - - - - | 1, 4 | 0x4F

	/* Arithmetic 16-bit Instructions */
	void add_hl_r16(Registers reg); // - 0 H C | 1, 8 | 0x09, 0x19, 0x29, 0x39
	void add_sp_e8();               // 0 0 H C | 2, 16 | 0xE8
	void dec_r16(Registers reg);    // - - - - | 1, 8 | 0x0B, 0x1B, 0x2B, 0x3B
	void inc_r16(Registers reg);    // - - - - | 1, 8 | 0x03, 0x13, 0x23, 0x33

	/* Arithmetic 8-bit Instructions */
	void inc_r8(Registers reg);     // Z 0 H - | 1, 4 | 0x04, 0x14, 0x24, 0x0C, 0x1C, 0x2C, 0x3C
	void inc_hl();                  // Z 0 H - | 1, 12 | 0x34
	void dec_r8(Registers reg);     // Z 1 H - | 1, 4 | 0x05, 0x15, 0x25, 0x0D, 0x1D, 0x2D, 0x3D
	void dec_hl();                  // Z 1 H - | 1, 12 | 0x35
	void add_a_r8(Registers reg);   // Z 0 H C | 1, 4 | 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87
	void add_a_hl();                // Z 0 H C | 1, 8 | 0x86
	void add_a_n8();                // Z 0 H C | 2, 8 | 0xC6
	void adc_a_r8(Registers reg);   // Z 0 H C | 1, 4 | 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F
	void adc_a_hl();                // Z 0 H C | 1, 8 | 0x8E
	void adc_a_n8();                // Z 0 H C | 2, 8 | 0xCE
	void sub_a_r8(Registers reg);   // Z 1 H C | 1, 4 | 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97
	void sub_a_hl();                // Z 1 H C | 1, 8 | 0x96
	void sub_a_n8();                // Z 1 H C | 2, 8 | 0xD6
	void sbc_a_r8(Registers reg);   // Z 1 H C | 1, 4 | 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9F
	void sbc_a_hl();                // Z 1 H C | 1, 8 | 0x9E
	void sbc_a_n8();                // Z 1 H C | 2, 8 | 0xDE
	void and_a_r8(Registers reg);   // Z 0 1 0 | 1, 4 | 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7
	void and_a_hl();                // Z 0 1 0 | 1, 8 | 0xA6
	void and_a_n8();                // Z 0 1 0 | 2, 8 | 0xE6
	void xor_a_r8(Registers reg);   // Z 0 0 0 | 1, 4 | 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAF
	void xor_a_hl();                // Z 0 0 0 | 1, 8 | 0xAE
	void xor_a_n8();                // Z 0 0 0 | 2, 8 | 0xEE
	void or_a_r8(Registers reg);    // Z 0 0 0 | 1, 4 | 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7
	void or_a_hl();                 // Z 0 0 0 | 1, 8 | 0xB6
	void or_a_n8();                 // Z 0 0 0 | 2, 8 | 0xF6
	void cp_a_r8(Registers reg);    // Z 1 H C | 1, 4 | 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBF
	void cp_a_hl();                 // Z 1 H C | 1, 8 | 0xBE
	void cp_a_n8();                 // Z 1 H C | 2, 8 | 0xFE
	void daa();                     // Z - 0 C | 1, 4 | 0x27
	void cpl();                     // - 1 1 - | 1, 4 | 0x2F
	void scf();                     // - 0 0 1 | 1, 4 | 0x37
	void ccf();                     // - 0 0 C | 1, 4 | 0x3F

public:
	Instructions() = default;
	~Instructions() = default;
};

class OpcodeMap : public Instructions {
private:
	using Instructions = std::function<void(Registers)>;

	Instructions instructions[256]    = {};
	Instructions cb_instructions[256] = {};

public:
	OpcodeMap() {
		init_instructions();
	};
	~OpcodeMap();

	void init_instructions();
	void execute(uint8_t op_code);
};

#endif //AYB_OPERATOR_H
