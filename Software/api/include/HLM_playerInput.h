#ifndef _LALEMA_PLAYERINPUT_H_
#define _LALEMA_PLAYERINPUT_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum controllerType_e {
	CONTROLLERTYPE_NONE, // Controller not present
	CONTROLLERTYPE_FULL, // whole console as one player
	CONTROLLERTYPE_LEFT, // left (dpad) half of console as one player
	CONTROLLERTYPE_RIGHT, // right (action) half of console as one player
	CONTROLLERTYPE_KEYBOARD, // Computer Keyboard
	CONTROLLERTYPE_EXT, // externally connected controller such as a phone
} controllerType_e;

uint8_t getPlayerCount(void);
bool isConnected(uint8_t playernum);
controllerType_e getControllerType(uint8_t);

//Action Buttons
bool isPrimaryButtonPressed(uint8_t playernum, bool firstTime); // A
bool isSecondaryButtonPressed(uint8_t playernum, bool firstTime); // B
bool isCoPrimaryButtonPressed(uint8_t playernum, bool firstTime); // X
bool isCoSecondaryButtonPressed(uint8_t playernum, bool firstTime); // Y

bool hasDedicatedHomeButton(uint8_t playernum);
bool isHomeButtonPressed(uint8_t playernum, bool firstTime);

//start / select
bool isMenuButtonPressed(uint8_t playernum, bool firstTime); //any
bool hasLMenuButton(uint8_t playernum);
bool isLMenuButtonPressed(uint8_t playernum, bool firstTime); // Select
bool hasRMenuButton(uint8_t playernum);
bool isRMenuButtonPressed(uint8_t playernum, bool firstTime); // Start

//Shoulder Buttons
bool hasShoulderButton(uint8_t playernum);
bool isShoulderButtonPressed(uint8_t playernum, bool firstTime); //any
bool hasLShoulderButton(uint8_t playernum);
bool isLShoulderButtonPressed(uint8_t playernum, bool firstTime); // L
bool hasRShoulderButton(uint8_t playernum);
bool isRShoulderButtonPressed(uint8_t playernum, bool firstTime); // R

//Combined from dpad/joystick
int8_t getLRInput(uint8_t playernum);
int8_t getUDInput(uint8_t playernum);
bool getLInput(uint8_t playernum , bool firstTime);
bool getRInput(uint8_t playernum , bool firstTime);
bool getUInput(uint8_t playernum , bool firstTime);
bool getDInput(uint8_t playernum , bool firstTime);
bool scrollLInput(uint8_t playernum, uint16_t ms_first, uint16_t ms_fast);
bool scrollRInput(uint8_t playernum, uint16_t ms_first, uint16_t ms_fast);
bool scrollUInput(uint8_t playernum, uint16_t ms_first, uint16_t ms_fast);
bool scrollDInput(uint8_t playernum, uint16_t ms_first, uint16_t ms_fast);

//dpad
bool hasDPad(uint8_t playernum);
bool isDpadLeftButtonPressed(uint8_t playernum , bool firstTime);
bool isDpadUpButtonPressed(uint8_t playernum , bool firstTime);
bool isDpadRightButtonPressed(uint8_t playernum , bool firstTime);
bool isDpadDownButtonPressed(uint8_t playernum , bool firstTime);

//main joystick (left)
bool hasLJoystick(uint8_t playernum);
bool isLJoystickPressed(uint8_t playernum , bool firstTime);
int8_t getLJoystickXAxis(uint8_t playernum);
int8_t getLJoystickYAxis(uint8_t playernum);

//rotation joystick (right)
bool hasRJoystick(uint8_t playernum);
bool isRJoystickPressed(uint8_t playernum, bool firstTime);
int8_t getRJoystickXAxis(uint8_t playernum);
int8_t getRJoystickYAxis(uint8_t playernum);



#endif 
