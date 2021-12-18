#include "powermanagement.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "setup.h"
#include "adcMultipin.h"
#include <avr/delay.h>

volatile bool interrupt_happend = false;

//volatile uint32_t Boot_Key __attribute__ ((section (".noinit")));
//#define MAGIC_BOOT_KEY            0xDC42ACCA

void PowerLatch(void) {
	PIN_KEEPALIVE_reg.DIRSET = PIN_KEEPALIVE_bm; //pin PD3 (Keepalive) as output
	//if (Boot_Key != MAGIC_BOOT_KEY)
	if (RST.STATUS == RST_PORF_bm)
		PIN_KEEPALIVE_reg.OUTSET = PIN_KEEPALIVE_bm; //pin PD3 high
	else
		PIN_KEEPALIVE_reg.OUTCLR = PIN_KEEPALIVE_bm; //pin PD3 low

	PIN_PWRBTN_reg.DIRCLR = PIN_PWRBTN_bm; //pin PC4 (pwrbtn) as input
	PIN_PWRBTN_reg.PASTE3(PIN,PIN_PWRBTN_bit,CTRL) = PORT_OPC_PULLUP_gc; //pullup

	PIN_CRG_reg.DIRCLR = PIN_CRG_bm;
	PIN_CRG_reg.PASTE3(PIN,PIN_CRG_bit,CTRL) = PORT_OPC_PULLUP_gc;

	PIN_PWRBTN_reg.PIN4CTRL |= PORT_ISC_FALLING_gc;
	PIN_PWRBTN_reg.INT0MASK |= PIN_PWRBTN_bm;
	PIN_PWRBTN_reg.INTCTRL = PORT_INT0LVL_LO_gc;
	
	PORTC.DIRCLR = PIN3_bm; // Bootloader seems to do strage stuff with its pin
	
	SetupAdcMultipin();
}

void PowerWorker()
{
	
}


uint16_t getBatteryLevel()
{
	return getAnalogValue(PIN_BATTERYDIV_bit);
}

bool isPowerButtonPressed()
{
	return ((PIN_PWRBTN_reg.IN & PIN_PWRBTN_bm) == 0);
}

bool isDeviceCharging() {
	return ((PIN_CRG_reg.IN & PIN_CRG_bm) == 0);
}


//Power-BTN
ISR(PORTD_INT0_vect)
{
	if (interrupt_happend == true) {
		_delay_ms(100);
		PIN_KEEPALIVE_reg.OUTCLR = PIN_KEEPALIVE_bm; //pin PD3 (Keepalive) low
	}
	interrupt_happend = true;
	PIN_PWRBTN_reg.PIN4CTRL |= PORT_ISC_RISING_gc;
	//Boot_Key = MAGIC_BOOT_KEY;
}
