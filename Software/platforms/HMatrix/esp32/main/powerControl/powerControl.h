#ifndef _LALEMA_POWERCONTROL_H_
#define _LALEMA_POWERCONTROL_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void SetupPowerControl(void);

bool isDeviceCharging(void);

uint16_t getBatteryMillivolts(void);
uint8_t getBatteryPercentage(void);

#ifdef __cplusplus
}
#endif

#endif
