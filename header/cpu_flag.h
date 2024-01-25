#ifndef AYB_CPU_FLAG_H
#define AYB_CPU_FLAG_H

#include "cstdint"

#include "cpu_reg.h"

class CpuFlags : public CpuRegisters {
private:
	CpuRegisters& cpu_reg;

	static constexpr uint8_t Z_FLAG  = 0x80;
	static constexpr uint8_t N_FLAG  = 0x40;
	static constexpr uint8_t HC_FLAG = 0x20;
	static constexpr uint8_t CY_FLAG = 0x10;

	bool Z{};
	bool N{};
	bool HC{};
	bool CY{};
	bool IME{};
	bool HALT{};
	bool STOP{};

	bool interrupts_enable{};
	bool interrupts_pending_enable{};

public:
	explicit CpuFlags(CpuRegisters& cpu_reg) : cpu_reg(cpu_reg) {}

	~CpuFlags() = default;

	[[nodiscard]] bool get_z() const;
	[[nodiscard]] bool get_n() const;
	[[nodiscard]] bool get_hc() const;
	[[nodiscard]] bool get_cy() const;
	[[nodiscard]] bool get_ime() const;
	[[nodiscard]] bool get_halt() const;
	[[nodiscard]] bool get_stop() const;
	[[nodiscard]] bool get_enable_interrupts() const;
	[[nodiscard]] bool get_interrupts_pending_enable() const;


	void set_z(bool val);
	void set_n(bool val);
	void set_hc(bool val);
	void set_cy(bool val);
	void set_ime(bool val);
	void set_halt(bool val);
	void set_stop(bool val);
	void set_enable_interrupts(bool val);
	void set_interrupts_pending_enable(bool val);
};

#endif //AYB_CPU_FLAG_H
