
#include "espComm.h"
#include "VirtualSerial.h"
#include "powermanagement.h"
#include "joystick.h"

struct statusReportData_t {
	uint8_t raw[0];
	uint16_t buttonData;
	uint8_t joystickXAxis;
	uint8_t joystickYAxis;
	uint8_t joystickRXAxis;
	uint8_t joystickRYAxis;
	uint16_t batteryLevel;
	uint8_t checksumH;
	uint8_t checksumL;
};

//char msgggg[] = "Hallo\r\nWelt\r\ndies\r\nist\r\nein\r\nTest\r\n";

void SetupComm()
{
	
}


void SendDeviceStatus()
{
	struct statusReportData_t data;
	
	data.buttonData = getPressedButtons();
	if (isJoystickPressed())
		data.buttonData |= BUTTON_JL;
	if (isJoystickRPressed())
		data.buttonData |= BUTTON_JR;
	if (isPowerButtonPressed())
		data.buttonData |= BUTTON_PWR;
	if (isDeviceCharging())
		data.buttonData |= BUTTON_CRG;
	
	data.joystickXAxis = getJoystickXAxis();
	data.joystickYAxis = getJoystickYAxis();
	data.joystickRXAxis = getJoystickRXAxis();
	data.joystickRYAxis = getJoystickRYAxis();
	
	data.batteryLevel = getBatteryLevel();
	
	//Checksum
	CRC.CTRL = CRC_RESET_RESET1_gc;
	CRC.CTRL |= CRC_SOURCE_IO_gc;
	for (uint8_t i = 0; i < sizeof(data) - 2; i++) {
		CRC.DATAIN = data.raw[i];
	}
	CRC.STATUS |= CRC_BUSY_bm;
	while((CRC.STATUS & CRC_BUSY_bm));
	data.checksumL = CRC.CHECKSUM0;
	data.checksumH = CRC.CHECKSUM1; 
	
	//inject_data_usb(data.raw, sizeof(data));
	inject_data_uart(data.raw, sizeof(data));
	//inject_data_uart(msgggg, sizeof(msgggg) - 1);
}
