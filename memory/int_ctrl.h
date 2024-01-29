#ifndef AYB_INT_CTRL_H
#define AYB_INT_CTRL_H

#include "cstdint"
#include "stdexcept"

#include "bit_util.h"
#include "hw_reg.h"

enum class InterruptType {
	VBLANK,
	LCD_STAT,
	TIMER,
	SERIAL,
	JOYPAD
};

class InterruptController {
private:
	HardwareRegisters hw_reg;

public:
	InterruptController() = default;
	~InterruptController() = default;

	void request_interrupt(InterruptType type);
};

#endif //AYB_INT_CTRL_H
