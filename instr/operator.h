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
};

class Instructions : public Operator {
protected:
	// Arithmetic 16-bit
	void add_hl(Registers reg); // - 0 H C , Cycles: 2 , Size: 1
	void dec_16(Registers reg); // - - - - , Cycles: 2 , Size: 1
	void inc_16(Registers reg); // - - - - , Cycles: 2 , Size: 1

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
