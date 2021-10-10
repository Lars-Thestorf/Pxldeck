#include "playerInput.h"
#include "../xmegaComm/xmegaComm.h"

controllerType_e controllers[8] = {CONTROLLERTYPE_FULL, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE};

void cycleInputMethods() {
	if (controllers[0] == CONTROLLERTYPE_FULL) {
		controllers[0] = CONTROLLERTYPE_LEFT;
		controllers[1] = CONTROLLERTYPE_RIGHT;
	}
	else if (controllers[0] == CONTROLLERTYPE_LEFT && controllers[1] == CONTROLLERTYPE_RIGHT) {
		controllers[1] = CONTROLLERTYPE_NONE;
	}
	else if (controllers[0] == CONTROLLERTYPE_LEFT && controllers[1] == CONTROLLERTYPE_NONE) {
		controllers[0] = CONTROLLERTYPE_RIGHT;
	}
	else {
		controllers[0] = CONTROLLERTYPE_FULL;
	}
}

uint8_t getPlayerCount()
{
	uint8_t players = 0;
	while (controllers[players] != CONTROLLERTYPE_NONE && players < 8)
		players++;
	return players;
}
bool isConnected ( uint8_t playernum )
{
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE;
}
controllerType_e getControllerType ( uint8_t playernum )
{
	return controllers[playernum - 1];
}

bool isPrimaryButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_A) > 0;
		case CONTROLLERTYPE_LEFT:
			return (xmegaGetPressedButtons() & BUTTON_DD) > 0;
		case CONTROLLERTYPE_RIGHT: 
			return (xmegaGetPressedButtons() & BUTTON_X) > 0;
		default:
			return false;
	}
}
bool isSecondaryButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_B) > 0;
		case CONTROLLERTYPE_LEFT:
			return (xmegaGetPressedButtons() & BUTTON_DL) > 0;
		case CONTROLLERTYPE_RIGHT: 
			return (xmegaGetPressedButtons() & BUTTON_A) > 0;
		default:
			return false;
	}
}
bool isCoPrimaryButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_X) > 0;
		case CONTROLLERTYPE_LEFT:
			return (xmegaGetPressedButtons() & BUTTON_DR) > 0;
		case CONTROLLERTYPE_RIGHT: 
			return (xmegaGetPressedButtons() & BUTTON_Y) > 0;
		default:
			return false;
	}
}
bool isCoSecondaryButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_Y) > 0;
		case CONTROLLERTYPE_LEFT:
			return (xmegaGetPressedButtons() & BUTTON_DU) > 0;
		case CONTROLLERTYPE_RIGHT: 
			return (xmegaGetPressedButtons() & BUTTON_B) > 0;
		default:
			return false;
	}
}

bool hasDedicatedHomeButton ( uint8_t playernum )
{
	return false;
}
bool isHomeButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & (BUTTON_START | BUTTON_SELECT)) == (BUTTON_START | BUTTON_SELECT);
		case CONTROLLERTYPE_LEFT:
			return (xmegaGetPressedButtons() & (BUTTON_SELECT | BUTTON_L)) == (BUTTON_SELECT | BUTTON_L);
		case CONTROLLERTYPE_RIGHT: 
			return (xmegaGetPressedButtons() & (BUTTON_START | BUTTON_R)) == (BUTTON_START | BUTTON_R);
		default:
			return false;
	}
}

//Shoulder buttons
bool hasShoulderButton( uint8_t playernum ){
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE;
}
bool isShoulderButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & (BUTTON_L | BUTTON_R)) > 0;
		case CONTROLLERTYPE_LEFT:
			return (xmegaGetPressedButtons() & BUTTON_L) > 0;
		case CONTROLLERTYPE_RIGHT: 
			return (xmegaGetPressedButtons() & BUTTON_R) > 0;
		default:
			return false;
	}
}
bool hasLShoulderButton ( uint8_t playernum )
{
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE && controllers[playernum - 1] != CONTROLLERTYPE_RIGHT;
}
bool isLShoulderButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_L) > 0;
		case CONTROLLERTYPE_LEFT:
			return (xmegaGetPressedButtons() & BUTTON_L) > 0;
		default:
			return false;
	}
}
bool hasRShoulderButton ( uint8_t playernum )
{
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE && controllers[playernum - 1] != CONTROLLERTYPE_LEFT;
}
bool isRShoulderButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_R) > 0;
		case CONTROLLERTYPE_RIGHT: 
			return (xmegaGetPressedButtons() & BUTTON_R) > 0;
		default:
			return false;
	}
}


bool isMenuButtonPressed(uint8_t playernum) {
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & (BUTTON_START | BUTTON_SELECT)) > 0;
		case CONTROLLERTYPE_LEFT:
			return (xmegaGetPressedButtons() & BUTTON_SELECT) > 0;
		case CONTROLLERTYPE_RIGHT: 
			return (xmegaGetPressedButtons() & BUTTON_START) > 0;
		default:
			return false;
	}
}
bool hasLMenuButton(uint8_t playernum) {
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE && controllers[playernum - 1] != CONTROLLERTYPE_LEFT;
}
bool isLMenuButtonPressed(uint8_t playernum) {
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_SELECT) > 0;
		case CONTROLLERTYPE_RIGHT: 
			return (xmegaGetPressedButtons() & BUTTON_START) > 0;
		default:
			return false;
	}
}
bool hasRMenuButton(uint8_t playernum) {
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE && controllers[playernum - 1] != CONTROLLERTYPE_RIGHT;
}
bool isRMenuButtonPressed(uint8_t playernum) {
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_START) > 0;
		case CONTROLLERTYPE_LEFT:
			return (xmegaGetPressedButtons() & BUTTON_SELECT) > 0;
		default:
			return false;
	}
}

int8_t getLRInput ( uint8_t playernum )
{
	uint8_t value = getJoystickXAxis(playernum);
	if (isDpadLeftButtonPressed(playernum))
		value = -127;
	if (isDpadRightButtonPressed(playernum))
		value = +127;
	return value;
}
int8_t getUDInput ( uint8_t playernum )
{
	uint8_t value = getJoystickYAxis(playernum);
	if (isDpadUpButtonPressed(playernum))
		value = -127;
	if (isDpadDownButtonPressed(playernum))
		value = +127;
	return value;
}

bool hasDPad ( uint8_t playernum )
{
	return controllers[playernum - 1] == CONTROLLERTYPE_FULL;
}
bool isDpadLeftButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_DL) > 0;
		default:
			return false;
	}
}
bool isDpadRightButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_DR) > 0;
		default:
			return false;
	}
}
bool isDpadUpButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_DU) > 0;
		default:
			return false;
	}
}
bool isDpadDownButtonPressed ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return (xmegaGetPressedButtons() & BUTTON_DD) > 0;
		default:
			return false;
	}
}

bool hasJoystick ( uint8_t playernum )
{
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE;
}
int8_t getJoystickXAxis ( uint8_t playernum )
{	
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return xmegaGetJoystickXAxis() - 128;
		case CONTROLLERTYPE_LEFT:
			return xmegaGetJoystickYAxis() - 128;
		case CONTROLLERTYPE_RIGHT:
			return -(xmegaGetJoystickRYAxis() - 128);
		default:
			return 0;
	}
}
int8_t getJoystickYAxis ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return xmegaGetJoystickYAxis() - 128;
		case CONTROLLERTYPE_LEFT:
			return -(xmegaGetJoystickXAxis() - 128);
		case CONTROLLERTYPE_RIGHT:
			return xmegaGetJoystickRXAxis() - 128;
		default:
			return 0;
	}
}

bool hasRJoystick ( uint8_t playernum )
{
	return controllers[playernum - 1] == CONTROLLERTYPE_FULL;
}
int8_t getRJoystickXAxis ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return xmegaGetJoystickRXAxis() - 128;
		default:
			return 0;
	}
}
int8_t getRJoystickYAxis ( uint8_t playernum )
{
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			return xmegaGetJoystickRYAxis() - 128;
		default:
			return 0;
	}
}
