#include "input.h"

uint16_t current_button_state;

void setButtonPressed(uint8_t button_idx, bool pressed)
{
	if (pressed)
		current_button_state |= 1 << button_idx;
	else
		current_button_state &= ~(1 << button_idx);
}

bool getButtonPressed(uint8_t button_idx)
{
	return ((current_button_state & (1 << button_idx)) != 0);
}

