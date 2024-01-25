#include "timer.h"

bool Timer::is_double_speed = false;

void Timer::timer_step(int cycles) {
	handle_div_register(cycles);
	handle_tima_register(cycles);
}
void Timer::handle_div_register(int cycles) {
	div_counter += cycles;
	if (div_counter < (is_double_speed ? DIV_CGB_FREQ : DIV_DMG_FREQ)) return;
	div_counter = 0;
	hw_reg.set_div(hw_reg.get_div() + 1);
}
void Timer::handle_tima_register(int cycles) {
	if (!mem_util.is_tac_timer_enabled()) return;
	tima_counter += cycles;
	if (tima_counter < get_timer_speed()) return;
	tima_counter = 0;
	if (hw_reg.get_tima() == 0xFF) {
		hw_reg.set_tima(hw_reg.get_tma()); // Overflow, so reset to TMA.
		int_ctrl.request_interrupt(InterruptType::TIMER);
	}
	else {
		hw_reg.set_tima(hw_reg.get_tima() + 1); // TIMA++
	}
}
int Timer::max_cycles(float fps) {
	return static_cast<int>(is_double_speed ? DOUBLE_FREQ / fps : (NORMAL_FREQ / fps));
}
void Timer::toggle_timer_speed() {
	is_double_speed = !is_double_speed;
}
int Timer::get_timer_speed() {
	return is_double_speed ? mem_util.get_tac_timer_freq() : mem_util.get_tac_timer_freq() * 2;
}
