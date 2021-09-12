#ifndef _LALEMA_JOYSTICKS_H_
#define _LALEMA_JOYSTICKS_H_

#include <stdbool.h>
#include <stdint.h>

void SetupJoysticks();

bool isJoystickPressed();
bool isJoystickRPressed();
uint8_t getJoystickXAxis();
uint8_t getJoystickYAxis();
uint8_t getJoystickRXAxis();
uint8_t getJoystickRYAxis();

#endif


