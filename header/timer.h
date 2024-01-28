#ifndef AYB_TIMER_H
#define AYB_TIMER_H

#include "cstdint"
#include "vector"

#include "../memory/hw_reg.h"
#include "../memory/mem_util.h"
#include "int_ctrl.h"
#include "mmu.h"

class Timer {
private:
//	Mmu                 mmu;
	HardwareRegisters   hw_reg;
	MemoryUtility       mem_util;
	InterruptController int_ctrl;

	static const int NORMAL_FREQ  = 4194304;
	static const int DOUBLE_FREQ  = 8388608;
	const int        DIV_DMG_FREQ = 256;
	const int        DIV_CGB_FREQ = 512;
	int              div_counter  = 0;
	int              tima_counter = 0;

	static bool is_double_speed;

	std::vector<int> timer_speeds = {
			1024,
			16,
			64,
			256
	};

	std::vector<int> timer_speeds_double = {
			2048,
			32,
			128,
			512
	};

public:
	Timer() = default;
	~Timer() = default;

	void timer_step(int cycles);
	void handle_div_register(int cycles);
	void handle_tima_register(int cycles);
	static int max_cycles(float fps);
	static void toggle_timer_speed();
	int get_timer_speed();
	bool is_double_speed_enabled() { return is_double_speed; }
};

#endif //AYB_TIMER_H
