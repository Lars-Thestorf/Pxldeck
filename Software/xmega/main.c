#include "VirtualSerial.h"
#include "powermanagement.h"
#include "buttonMatrix.h"

#include <avr/wdt.h>
#include <stdio.h>
#include "joystick.h"

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
}

char logmsg[] = "[xmega] ------------------- xxxx yyyy rxrx ryry \r\n";
char logmsg2[] = "[xmega] Hallo ESP, kannst du mich hören?";

void DebugWorker(void) {
	static uint16_t supermessage = 0;
	supermessage++;
	supermessage &= 0x07FF;
	if (supermessage == 0) {
		buttons_t states = getPressedButtons();
		
		for (int i = 0; i < 16; i++) {
			if (states & 1) {
				logmsg[8+i+(i/4)] = '1';
			} else {
				logmsg[8+i+(i/4)] = '0';
			}
			states >>= 1;
		}
		
		uint8_t joystickX = getJoystickXAxis();
		uint8_t joystickY = getJoystickYAxis();
		uint8_t joystickRX = getJoystickRXAxis();
		uint8_t joystickRY = getJoystickRYAxis();
		sprintf(logmsg + 28, "x%d y%d\r\nx%d y%d\r\n", joystickX, joystickY, joystickRX, joystickRY);
		
		inject_message_usb(logmsg);
		inject_message_uart(logmsg + 8);
	}
}

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	wdt_enable( WDT_PER_4KCLK_gc );
	
	PowerLatch();
	SetupHardware();
	
	SetupVirtualSerial();
	SetupButtonMatrix();
	SetupJoysticks();
	
	wdt_reset();
	wdt_enable( WDT_PER_1KCLK_gc );
	
	for (;;)
	{
		VirtualSerialWorker();
		ButtonMatrixWorker();
		DebugWorker();
		if(!isPowerButtonPressed()) {
			wdt_reset();
		}
	}
}
