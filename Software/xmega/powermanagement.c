#include "powermanagement.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "setup.h"

volatile bool interrupt_happend = false;

void PowerLatch(void) {
	PIN_KEEPALIVE_reg.DIRSET = PIN_KEEPALIVE_bm; //pin PD3 (Keepalive) as output
	PIN_KEEPALIVE_reg.OUTSET = PIN_KEEPALIVE_bm; //pin PD3 high

	PIN_PWRBTN_reg.DIRCLR = PIN_PWRBTN_bm; //pin PC4 (pwrbtn) as input
	PIN_PWRBTN_reg.PASTE3(PIN,PIN_PWRBTN_bit,CTRL) = PORT_OPC_PULLUP_gc; //pullup

	PIN_CRG_reg.DIRCLR = PIN_CRG_bm;
	PIN_CRG_reg.PASTE3(PIN,PIN_CRG_bit,CTRL) = PORT_OPC_PULLUP_gc;

	PIN_PWRBTN_reg.PIN4CTRL |= PORT_ISC_RISING_gc;
	PIN_PWRBTN_reg.INT0MASK |= PIN_PWRBTN_bm;
	PIN_PWRBTN_reg.INTCTRL = PORT_INT0LVL_LO_gc;
}

bool isPowerButtonPressed()
{
	return ((PIN_PWRBTN_reg.IN & PIN_PWRBTN_bm) == 0);
}


//Power-BTN
ISR(PORTD_INT0_vect)
{
	PIN_KEEPALIVE_reg.OUTCLR = PIN_KEEPALIVE_bm; //pin PD3 (Keepalive) low
	interrupt_happend = true;
}
