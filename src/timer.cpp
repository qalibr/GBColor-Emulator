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
	mmu.get_hw_reg().set_div(mmu.get_hw_reg().get_div() + 1);
}
void Timer::handle_tima_register(int cycles) {
	if (!mmu.get_mem().is_tac_timer_enabled()) return;
	tima_counter += cycles;
	int speed = get_timer_speed();
	if (tima_counter >= speed) {
		tima_counter -= speed; // Decrement to handle multiple increments in one cycle.
		if (mmu.get_hw_reg().get_tima() == 0xFF) {
			mmu.get_hw_reg().set_tima(mmu.get_hw_reg().get_tima()); // Overflow so set to TMA.
			mmu.get_int_ctrl().request_interrupt(InterruptType::TIMER);
		}
		else {
			mmu.get_hw_reg().set_tima(mmu.get_hw_reg().get_tima() + 1); // TIMA++
		}
	}
}
int Timer::max_cycles(float fps) {
	return static_cast<int>(is_double_speed ? DOUBLE_FREQ / fps : (NORMAL_FREQ / fps));
}
void Timer::toggle_timer_speed() {
	is_double_speed = !is_double_speed;
}
int Timer::get_timer_speed() {
	int timer_freq = mmu.get_mem().get_tac_timer_freq();
	return is_double_speed ? timer_freq / 2 : timer_freq;
}
