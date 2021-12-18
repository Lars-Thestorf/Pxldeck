#ifndef _LALEMA_JOYSTICKS_H_
#define _LALEMA_JOYSTICKS_H_

#include <stdbool.h>
#include <stdint.h>

void SetupJoysticks(void);

bool isJoystickPressed(void);
bool isJoystickRPressed(void);
uint8_t getJoystickXAxis(void);
uint8_t getJoystickYAxis(void);
uint8_t getJoystickRXAxis(void);
uint8_t getJoystickRYAxis(void);

#endif


