#include "buttonMatrix.h"
#include <avr/io.h>
#include "setup.h"
#include <util/delay.h>

buttons_t buttons_cache;

void SetupButtonMatrix()
{
	//Inputs
	PIN_BTNMATRIX_1_reg.DIRCLR = PIN_BTNMATRIX_1_bm;
	PIN_BTNMATRIX_1_reg.PASTE3(PIN,PIN_BTNMATRIX_1_bit,CTRL) = PORT_OPC_PULLUP_gc;
	PIN_BTNMATRIX_2_reg.DIRCLR = PIN_BTNMATRIX_2_bm;
	PIN_BTNMATRIX_2_reg.PASTE3(PIN,PIN_BTNMATRIX_2_bit,CTRL) = PORT_OPC_PULLUP_gc;
	PIN_BTNMATRIX_3_reg.DIRCLR = PIN_BTNMATRIX_3_bm;
	PIN_BTNMATRIX_3_reg.PASTE3(PIN,PIN_BTNMATRIX_3_bit,CTRL) = PORT_OPC_PULLUP_gc;
	PIN_BTNMATRIX_4_reg.DIRCLR = PIN_BTNMATRIX_4_bm;
	PIN_BTNMATRIX_4_reg.PASTE3(PIN,PIN_BTNMATRIX_4_bit,CTRL) = PORT_OPC_PULLUP_gc;
	
	PIN_JOYSTICK_L_BTN_reg.DIRCLR = PIN_JOYSTICK_L_BTN_bm;
	PIN_JOYSTICK_L_BTN_reg.PASTE3(PIN,PIN_JOYSTICK_L_BTN_bit,CTRL) = PORT_OPC_PULLUP_gc;
	PIN_JOYSTICK_R_BTN_reg.DIRCLR = PIN_JOYSTICK_R_BTN_bm;
	PIN_JOYSTICK_R_BTN_reg.PASTE3(PIN,PIN_JOYSTICK_R_BTN_bit,CTRL) = PORT_OPC_PULLUP_gc;
	
	PORTC.DIRCLR = PIN3_bm;
	
	//Outputs
	PIN_BTNMATRIX_A_reg.DIRSET = PIN_BTNMATRIX_A_bm;
	PIN_BTNMATRIX_A_reg.OUTSET = PIN_BTNMATRIX_A_bm;
	PIN_BTNMATRIX_B_reg.DIRSET = PIN_BTNMATRIX_B_bm;
	PIN_BTNMATRIX_B_reg.OUTSET = PIN_BTNMATRIX_B_bm;
	PIN_BTNMATRIX_C_reg.DIRSET = PIN_BTNMATRIX_C_bm;
	PIN_BTNMATRIX_C_reg.OUTSET = PIN_BTNMATRIX_C_bm;
}
void ButtonMatrixWorker()
{
	buttons_t buttonstates = 0;
	
	PIN_BTNMATRIX_A_reg.OUTCLR = PIN_BTNMATRIX_A_bm;
	_delay_us(10);
	if ((PIN_BTNMATRIX_1_reg.IN & PIN_BTNMATRIX_1_bm) == 0)
		buttonstates |= BUTTON_L;
	if ((PIN_BTNMATRIX_2_reg.IN & PIN_BTNMATRIX_2_bm) == 0)
		buttonstates |= BUTTON_SELECT;
	if ((PIN_BTNMATRIX_3_reg.IN & PIN_BTNMATRIX_3_bm) == 0)
		buttonstates |= BUTTON_START;
	if ((PIN_BTNMATRIX_4_reg.IN & PIN_BTNMATRIX_4_bm) == 0)
		buttonstates |= BUTTON_R;
	PIN_BTNMATRIX_A_reg.OUTSET = PIN_BTNMATRIX_A_bm;
	
	PIN_BTNMATRIX_B_reg.OUTCLR = PIN_BTNMATRIX_B_bm;
	_delay_us(10);
	if ((PIN_BTNMATRIX_1_reg.IN & PIN_BTNMATRIX_1_bm) == 0)
		buttonstates |= BUTTON_DL;
	if ((PIN_BTNMATRIX_2_reg.IN & PIN_BTNMATRIX_2_bm) == 0)
		buttonstates |= BUTTON_DD;
	if ((PIN_BTNMATRIX_3_reg.IN & PIN_BTNMATRIX_3_bm) == 0)
		buttonstates |= BUTTON_B;
	if ((PIN_BTNMATRIX_4_reg.IN & PIN_BTNMATRIX_4_bm) == 0)
		buttonstates |= BUTTON_A;
	PIN_BTNMATRIX_B_reg.OUTSET = PIN_BTNMATRIX_B_bm;
	
	PIN_BTNMATRIX_C_reg.OUTCLR = PIN_BTNMATRIX_C_bm;
	_delay_us(10);
	if ((PIN_BTNMATRIX_1_reg.IN & PIN_BTNMATRIX_1_bm) == 0)
		buttonstates |= BUTTON_DU;
	if ((PIN_BTNMATRIX_2_reg.IN & PIN_BTNMATRIX_2_bm) == 0)
		buttonstates |= BUTTON_DR;
	if ((PIN_BTNMATRIX_3_reg.IN & PIN_BTNMATRIX_3_bm) == 0)
		buttonstates |= BUTTON_Y;
	if ((PIN_BTNMATRIX_4_reg.IN & PIN_BTNMATRIX_4_bm) == 0)
		buttonstates |= BUTTON_X;
	PIN_BTNMATRIX_C_reg.OUTSET = PIN_BTNMATRIX_C_bm;
	
	if ((PIN_JOYSTICK_L_BTN_reg.IN & PIN_JOYSTICK_L_BTN_bm) == 0)
		buttonstates |= BUTTON_JL;
	if ((PIN_JOYSTICK_R_BTN_reg.IN & PIN_JOYSTICK_R_BTN_bm) == 0)
		buttonstates |= BUTTON_JR;
	if ((PIN_CRG_reg.IN & PIN_CRG_bm) == 0)
		buttonstates |= BUTTON_CRG;
	if ((PIN_PWRBTN_reg.IN & PIN_PWRBTN_bm) == 0)
		buttonstates |= BUTTON_PWR;
	
	buttons_cache = buttonstates;
}

buttons_t getPressedButtons()
{
	return buttons_cache;
}
