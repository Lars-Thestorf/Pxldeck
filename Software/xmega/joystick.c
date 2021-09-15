#include "joystick.h"
#include "setup.h"
#include "adcMultipin.h"
#include "avr/io.h"

void SetupJoysticks()
{
	//This Code assumes adcMultipin is already started so this is commented out
	//SetupAdcMultipin();
	
	PIN_JOYSTICK_L_BTN_reg.DIRCLR = PIN_JOYSTICK_L_BTN_bm;
	PIN_JOYSTICK_L_BTN_reg.PASTE3(PIN,PIN_JOYSTICK_L_BTN_bit,CTRL) = PORT_OPC_PULLUP_gc;
	PIN_JOYSTICK_R_BTN_reg.DIRCLR = PIN_JOYSTICK_R_BTN_bm;
	PIN_JOYSTICK_R_BTN_reg.PASTE3(PIN,PIN_JOYSTICK_R_BTN_bit,CTRL) = PORT_OPC_PULLUP_gc;
}

uint8_t normalizeJoystickValue(uint8_t val) {
	if (val <= 12) {
		return 0;
	}
	if (val >= 255 - 12){
		return 255;
	}
	return val - 12 + val * 0.05;
}

bool isJoystickPressed()
{
	return ((PIN_JOYSTICK_L_BTN_reg.IN & PIN_JOYSTICK_L_BTN_bm) == 0);
}

bool isJoystickRPressed()
{
	return ((PIN_JOYSTICK_R_BTN_reg.IN & PIN_JOYSTICK_R_BTN_bm) == 0);
}

uint8_t getJoystickXAxis()
{
	return normalizeJoystickValue(getAnalogValue(PIN_JOYSTICK_L_XAXIS_bit) >> 4);
}

uint8_t getJoystickYAxis()
{
	return 0xFF - (normalizeJoystickValue(getAnalogValue(PIN_JOYSTICK_L_YAXIS_bit) >> 4));
}

uint8_t getJoystickRXAxis()
{
	return 0xFF - (normalizeJoystickValue(getAnalogValue(PIN_JOYSTICK_R_XAXIS_bit) >> 4));
}

uint8_t getJoystickRYAxis()
{
	return normalizeJoystickValue(getAnalogValue(PIN_JOYSTICK_R_YAXIS_bit) >> 4);
}
