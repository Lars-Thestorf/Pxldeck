#include "joystick.h"
#include "setup.h"
#include "adcMultipin.h"
#include "avr/io.h"

//#define JOYSTICKS_RAW

void SetupJoysticks()
{
	//This Code assumes adcMultipin is already started so this is commented out
	//SetupAdcMultipin();
	
	PIN_JOYSTICK_L_BTN_reg.DIRCLR = PIN_JOYSTICK_L_BTN_bm;
	PIN_JOYSTICK_L_BTN_reg.PASTE3(PIN,PIN_JOYSTICK_L_BTN_bit,CTRL) = PORT_OPC_PULLUP_gc;
	PIN_JOYSTICK_R_BTN_reg.DIRCLR = PIN_JOYSTICK_R_BTN_bm;
	PIN_JOYSTICK_R_BTN_reg.PASTE3(PIN,PIN_JOYSTICK_R_BTN_bit,CTRL) = PORT_OPC_PULLUP_gc;
}

//Stick min     0x0b - 0x0c
//Stick center  0x7e - 0x8b
//Stick max     0xF8 - 0xFF
#define JOYSTICKS_MIN 0x0C
#define JOYSTICKS_CENTER_MIN 0x7E
#define JOYSTICKS_CENTER_MAX 0x8B
#define JOYSTICKS_MAX 0xF8
uint8_t normalizeJoystickValue(uint8_t val) {
	if (val >= JOYSTICKS_CENTER_MIN && val <= JOYSTICKS_CENTER_MAX){
		return 0x80;
	}
	if (val <= JOYSTICKS_MIN) {
		return 0x01;
	}
	if (val >= JOYSTICKS_MAX){
		return 0xFF;
	}
	if (val < JOYSTICKS_CENTER_MIN) {
		return (val - JOYSTICKS_MIN) * 0x80 / (JOYSTICKS_CENTER_MIN - JOYSTICKS_MIN);
	}
	else {
		return (val - JOYSTICKS_CENTER_MAX) * 0x80 / (JOYSTICKS_MAX - JOYSTICKS_CENTER_MAX) + 0x80;
	}
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
	#ifdef JOYSTICKS_RAW
		return getAnalogValue(PIN_JOYSTICK_L_XAXIS_bit) >> 4;
	#else
		return normalizeJoystickValue(getAnalogValue(PIN_JOYSTICK_L_XAXIS_bit) >> 4);
	#endif
}

uint8_t getJoystickYAxis()
{
	#ifdef JOYSTICKS_RAW
		return getAnalogValue(PIN_JOYSTICK_L_YAXIS_bit) >> 4;
	#else
		return 0x100 - (normalizeJoystickValue(getAnalogValue(PIN_JOYSTICK_L_YAXIS_bit) >> 4));
	#endif
}

uint8_t getJoystickRXAxis()
{
	#ifdef JOYSTICKS_RAW
		return getAnalogValue(PIN_JOYSTICK_R_XAXIS_bit) >> 4;
	#else
		return 0x100 - (normalizeJoystickValue(getAnalogValue(PIN_JOYSTICK_R_XAXIS_bit) >> 4));
	#endif
}

uint8_t getJoystickRYAxis()
{
	#ifdef JOYSTICKS_RAW
		return getAnalogValue(PIN_JOYSTICK_R_YAXIS_bit) >> 4;
	#else
		return normalizeJoystickValue(getAnalogValue(PIN_JOYSTICK_R_YAXIS_bit) >> 4);
	#endif
}
