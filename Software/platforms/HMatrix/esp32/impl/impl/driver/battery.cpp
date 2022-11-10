#include <HLM_battery.h>
#include "../xmegaComm/xmegaComm.h"

void SetupPowerControl()
{
	
}

uint16_t getBatteryMillivolts()
{
	// 4,125V ist ADC-max (4095)
	return xmegaGetBatteryLevel()*4125/4095;
}

uint8_t getBatteryPercentage()
{
	// 4,125V ist voll
	// 3,3V ist leer
	// 0,825V schwankung
	return (getBatteryMillivolts() - 3300)*100/825;
}


bool isBatteryCharging()
{
	return (xmegaGetPressedButtons() & BUTTON_CRG) > 0;
}

