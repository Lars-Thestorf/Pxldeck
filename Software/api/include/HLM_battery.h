#ifndef HLM_BATTERY_H_
#define HLM_BATTERY_H_

#include <stdbool.h>
#include <stdint.h>

bool hasBattery();
uint16_t getBatteryMillivolts();
uint8_t getBatteryPercentage();
bool isBatteryCharging();

#endif

