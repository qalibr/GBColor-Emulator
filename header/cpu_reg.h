#ifndef AYB_CPU_REG_H
#define AYB_CPU_REG_H

#include "cstdint"

class CpuRegisters {
protected:
	uint8_t  A;
	uint8_t  F;
	uint8_t  B;
	uint8_t  C;
	uint8_t  D;
	uint8_t  E;
	uint8_t  H;
	uint8_t  L;
	uint16_t SP;
	uint16_t PC;

public:
	CpuRegisters();

	~CpuRegisters() = default;

	[[nodiscard]] uint8_t get_a() const;
	[[nodiscard]] uint8_t get_f() const;
	[[nodiscard]] uint8_t get_b() const;
	[[nodiscard]] uint8_t get_c() const;
	[[nodiscard]] uint8_t get_d() const;
	[[nodiscard]] uint8_t get_e() const;
	[[nodiscard]] uint8_t get_h() const;
	[[nodiscard]] uint8_t get_l() const;
	[[nodiscard]] uint16_t get_af() const;
	[[nodiscard]] uint16_t get_bc() const;
	[[nodiscard]] uint16_t get_de() const;
	[[nodiscard]] uint16_t get_hl() const;
	[[nodiscard]] uint16_t get_sp() const;
	[[nodiscard]] uint16_t get_pc() const;

	void set_a(uint8_t val);
	void set_f(uint8_t val);
	void set_b(uint8_t val);
	void set_c(uint8_t val);
	void set_d(uint8_t val);
	void set_e(uint8_t val);
	void set_h(uint8_t val);
	void set_l(uint8_t val);
	void set_af(uint16_t val);
	void set_bc(uint16_t val);
	void set_de(uint16_t val);
	void set_hl(uint16_t val);
	void set_sp(uint16_t val);
	void add_sp(uint16_t val);
	void dec_sp(uint16_t val);
	void set_pc(uint16_t val);
	void add_pc(uint16_t val);
	void dec_pc(uint16_t val);
};

#endif //AYB_CPU_REG_H
