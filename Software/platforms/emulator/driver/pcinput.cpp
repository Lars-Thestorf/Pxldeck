#include <HLM_playerInput.h>
#include "../input.h"

uint8_t getPlayerCount()
{
	return 1;
}
controllerType_e getControllerType ( uint8_t playernum)
{
	if (playernum != 1) return CONTROLLERTYPE_NONE;
	return CONTROLLERTYPE_KEYBOARD;
}
bool isConnected ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return true;
}
bool hasJoystick ( uint8_t playernum )
{
	return false;
}
bool hasRJoystick ( uint8_t playernum )
{
	return false;
}
bool hasDPad ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return true;
}
bool hasDedicatedHomeButton ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return true;
}
bool hasRMenuButton ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return true;
}
bool hasLMenuButton ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return true;
}
bool hasShoulderButton ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return true;
}
bool isShoulderButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return isLShoulderButtonPressed(playernum) || isRShoulderButtonPressed(playernum);
}
bool hasLShoulderButton ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return true;
}
bool isLShoulderButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_LSHOULDER);
}
bool hasRShoulderButton ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return true;
}
bool isRShoulderButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_RSHOULDER);
}

bool isDpadUpButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_DU);
}
bool isDpadDownButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_DD);
}
bool isDpadRightButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_DR);
}
bool isDpadLeftButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_DL);
}

bool isPrimaryButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_PRIMARY);
}
bool isSecondaryButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_SECONDARY);
}
bool isCoPrimaryButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_COPRIMARY);
}
bool isCoSecondaryButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_COSECONDARY);
}

bool isHomeButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_HOME);
}
bool isMenuButtonPressed ( uint8_t playernum )
{
	return isLMenuButtonPressed(playernum) || isRMenuButtonPressed(playernum);
}
bool isLMenuButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_LMENU);
}
bool isRMenuButtonPressed ( uint8_t playernum )
{
	if (playernum != 1) return false;
	return getButtonPressed(BUTTON_RMENU);
}

bool isJoystickPressed ( uint8_t playernum )
{
	return false;
}
int8_t getJoystickXAxis ( uint8_t playernum )
{
	return 0;
}
int8_t getJoystickYAxis ( uint8_t playernum )
{
	return 0;
}
bool isRJoystickPressed ( uint8_t playernum )
{
	return false;
}
int8_t getRJoystickXAxis ( uint8_t playernum )
{
	return 0;
}
int8_t getRJoystickYAxis ( uint8_t playernum )
{
	return 0;
}

int8_t getLRInput(uint8_t playernum) {
	if (playernum != 1) return 0;
	if (getButtonPressed(BUTTON_DR))
		return 127;
	if (getButtonPressed(BUTTON_DL))
		return -127;
	return 0;
}
int8_t getUDInput(uint8_t playernum) {
	if (playernum != 1) return 0;
	if (getButtonPressed(BUTTON_DD))
		return 127;
	if (getButtonPressed(BUTTON_DU))
		return -127;
	return 0;
}
