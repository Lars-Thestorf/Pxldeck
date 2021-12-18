#ifndef _LALEMA_POWERMANAGEMENT_H_
#define _LALEMA_POWERMANAGEMENT_H_

#include <stdbool.h>
#include <stdint.h>

void PowerLatch(void);

void PowerWorker(void);

uint16_t getBatteryLevel(void);

bool isPowerButtonPressed(void);
bool isDeviceCharging(void);

#endif
