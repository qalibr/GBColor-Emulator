#include "int_ctrl.h"

void InterruptController::request_interrupt(InterruptType type) {
	switch (type) {
	case InterruptType::VBLANK:
		hw_reg.set_if(BitUtil::set_bit(hw_reg.get_if(), 0));
		break;
	case InterruptType::LCD_STAT:
		hw_reg.set_if(BitUtil::set_bit(hw_reg.get_if(), 1));
		break;
	case InterruptType::TIMER:
		hw_reg.set_if(BitUtil::set_bit(hw_reg.get_if(), 2));
		break;
	case InterruptType::SERIAL:
		hw_reg.set_if(BitUtil::set_bit(hw_reg.get_if(), 3));
		break;
	case InterruptType::JOYPAD:
		hw_reg.set_if(BitUtil::set_bit(hw_reg.get_if(), 4));
		break;
	default:
		throw std::runtime_error("Invalid interrupt type");
	}
}
