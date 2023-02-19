#ifndef _LALEMA_PLAYERINPUT_H_
#define _LALEMA_PLAYERINPUT_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_PLAYER_COUNT 8

typedef enum controllerType_e {
	CONTROLLERTYPE_NONE, // Controller not present
	CONTROLLERTYPE_FULL, // whole console as one player
	CONTROLLERTYPE_LEFT, // left (dpad) half of console as one player
	CONTROLLERTYPE_RIGHT, // right (action) half of console as one player
	CONTROLLERTYPE_KEYBOARD, // Computer Keyboard
	CONTROLLERTYPE_EXT, // externally connected controller such as a phone
} controllerType_e;

void cycleInputMethods(void); //This Method is not here to stay

uint8_t getPlayerCount(void);
bool isConnected(uint8_t playernum);
controllerType_e getControllerType(uint8_t);

//Action Buttons
bool isPrimaryButtonPressed(uint8_t playernum); // A
bool isSecondaryButtonPressed(uint8_t playernum); // B
bool isCoPrimaryButtonPressed(uint8_t playernum); // X
bool isCoSecondaryButtonPressed(uint8_t playernum); // Y

bool hasDedicatedHomeButton(uint8_t playernum);
bool isHomeButtonPressed(uint8_t playernum);

//start / select
bool isMenuButtonPressed(uint8_t playernum); //any
bool hasLMenuButton(uint8_t playernum);
bool isLMenuButtonPressed(uint8_t playernum); // Select
bool hasRMenuButton(uint8_t playernum);
bool isRMenuButtonPressed(uint8_t playernum); // Start

//Shoulder Buttons
bool hasShoulderButton(uint8_t playernum);
bool isShoulderButtonPressed(uint8_t playernum); //any
bool hasLShoulderButton(uint8_t playernum);
bool isLShoulderButtonPressed(uint8_t playernum); // L
bool hasRShoulderButton(uint8_t playernum);
bool isRShoulderButtonPressed(uint8_t playernum); // R

//Combined from dpad/joystick
int8_t getLRInput(uint8_t playernum);
int8_t getUDInput(uint8_t playernum);

//dpad
bool hasDPad(uint8_t playernum);
bool isDpadLeftButtonPressed(uint8_t playernum);
bool isDpadUpButtonPressed(uint8_t playernum);
bool isDpadRightButtonPressed(uint8_t playernum);
bool isDpadDownButtonPressed(uint8_t playernum);

//main joystick (left)
bool hasJoystick(uint8_t playernum);
bool isJoystickPressed(uint8_t playernum);
int8_t getJoystickXAxis(uint8_t playernum);
int8_t getJoystickYAxis(uint8_t playernum);

//rotation joystick (right)
bool hasRJoystick(uint8_t playernum);
bool isRJoystickPressed(uint8_t playernum);
int8_t getRJoystickXAxis(uint8_t playernum);
int8_t getRJoystickYAxis(uint8_t playernum);

#endif 
