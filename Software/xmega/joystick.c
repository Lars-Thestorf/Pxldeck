#include "joystick.h"
#include "setup.h"
#include "adcMultipin.h"

void SetupJoysticks()
{
	//adc_init();
	SetupAdcMultipin();
}

uint8_t getJoystickXAxis()
{
	return getAnalogValue(PIN_JOYSTICK_L_XAXIS_bit) >> 4;
}

uint8_t getJoystickYAxis()
{
	return getAnalogValue(PIN_JOYSTICK_L_YAXIS_bit) >> 4;
}

uint8_t getJoystickRXAxis()
{
	return 0xFF - (getAnalogValue(PIN_JOYSTICK_R_XAXIS_bit) >> 4);
}

uint8_t getJoystickRYAxis()
{
	return 0xFF - (getAnalogValue(PIN_JOYSTICK_R_YAXIS_bit) >> 4);
}
