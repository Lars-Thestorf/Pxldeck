#include "playerInput.h"
#include "../xmegaComm/xmegaComm.h"

uint8_t getPlayerCount()
{
	return 1;
}
bool isConnected ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return true;
}
controllerType_e getControllerType ( uint8_t playernum )
{
	if (playernum != 1)
		return CONTROLLERTYPE_NONE;
	return CONTROLLERTYPE_FULL;
}

bool isPrimaryButtonPressed ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_A) > 0;
}
bool isSecondaryButtonPressed ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_B) > 0;
}
bool isCoPrimaryButtonPressed ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_X) > 0;
}
bool isCoSecondaryButtonPressed ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_Y) > 0;
}

bool hasDedicatedHomeButton ( uint8_t playernum )
{
	return false;
}
bool isHomeButtonPressed ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & (BUTTON_START | BUTTON_SELECT)) == (BUTTON_START | BUTTON_SELECT);
}

bool isMenuButtonPressed(uint8_t playernum) {
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & (BUTTON_START | BUTTON_SELECT)) > 0;
}
bool hasLMenuButton(uint8_t playernum) {
	if (playernum != 1)
		return false;
	return true;
}
bool isLMenuButtonPressed(uint8_t playernum) {
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_SELECT) > 0;
}
bool hasRMenuButton(uint8_t playernum) {
	if (playernum != 1)
		return false;
	return true;
}
bool isRMenuButtonPressed(uint8_t playernum) {
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_START) > 0;
}

int8_t getLRInput ( uint8_t playernum )
{
	if (playernum != 1)
		return 0;
	uint8_t value = getJoystickXAxis(playernum);
	if (isDpadLeftButtonPressed(playernum))
		value = -127;
	if (isDpadRightButtonPressed(playernum))
		value = +127;
	return value;
}
int8_t getUDInput ( uint8_t playernum )
{
	if (playernum != 1)
		return 0;
	uint8_t value = getJoystickYAxis(playernum);
	if (isDpadUpButtonPressed(playernum))
		value = -127;
	if (isDpadDownButtonPressed(playernum))
		value = +127;
	return value;
}

bool hasDPad ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return true;
}
bool isDpadLeftButtonPressed ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_DL) > 0;
}
bool isDpadRightButtonPressed ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_DR) > 0;
}
bool isDpadUpButtonPressed ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_DU) > 0;
}
bool isDpadDownButtonPressed ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return (xmegaGetPressedButtons() & BUTTON_DD) > 0;
}

bool hasJoystick ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return true;
}
int8_t getJoystickXAxis ( uint8_t playernum )
{
	if (playernum != 1)
		return 0;
	return xmegaGetJoystickXAxis() - 128;
}
int8_t getJoystickYAxis ( uint8_t playernum )
{
	if (playernum != 1)
		return 0;
	return xmegaGetJoystickYAxis() - 128;
}

bool hasRJoystick ( uint8_t playernum )
{
	if (playernum != 1)
		return false;
	return true;
}
int8_t getRJoystickXAxis ( uint8_t playernum )
{
	if (playernum != 1)
		return 0;
	return xmegaGetJoystickRXAxis() - 128;
}
int8_t getRJoystickYAxis ( uint8_t playernum )
{
	if (playernum != 1)
		return 0;
	return xmegaGetJoystickRYAxis() - 128;
}
