#include <HLM_playerInput.h>
#include <HLM_time.h>
#include "../xmegaComm/xmegaComm.h"
#include <stdio.h>

static bool prevPressPrimary[8] = {0,0,0,0,0,0,0,0};
static bool prevPressSecondary[8] = {0,0,0,0,0,0,0,0};
static bool prevPressCoSecondary[8] = {0,0,0,0,0,0,0,0};
static bool prevPressCoPrimary[8] = {0,0,0,0,0,0,0,0};
static bool prevPressHome[8] = {0,0,0,0,0,0,0,0};
static bool prevPressMenu[8] = {0,0,0,0,0,0,0,0};
static bool prevPressLMenu[8] = {0,0,0,0,0,0,0,0};
static bool prevPressRMenu[8] = {0,0,0,0,0,0,0,0};
static bool prevPressShoulder[8] = {0,0,0,0,0,0,0,0};
static bool prevPressLShoulder[8] = {0,0,0,0,0,0,0,0};
static bool prevPressRShoulder[8] = {0,0,0,0,0,0,0,0};

static bool prevPressLeft[8] = {0,0,0,0,0,0,0,0};
static bool prevPressRight[8] = {0,0,0,0,0,0,0,0};
static bool prevPressUp[8] = {0,0,0,0,0,0,0,0};
static bool prevPressDown[8] = {0,0,0,0,0,0,0,0};

static bool prevPressLInput[8] = {0,0,0,0,0,0,0,0};
static bool prevPressRInput[8] = {0,0,0,0,0,0,0,0};
static bool prevPressUInput[8] = {0,0,0,0,0,0,0,0};
static bool prevPressDInput[8] = {0,0,0,0,0,0,0,0};

static bool prevPressLJoystick[8] = {0,0,0,0,0,0,0,0};
static bool prevPressRJoystick[8] = {0,0,0,0,0,0,0,0};

controllerType_e controllers[8] = {CONTROLLERTYPE_FULL, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE, CONTROLLERTYPE_NONE};

uint8_t getPlayerCount()
{
	uint8_t players = 0;
	while (controllers[players] != CONTROLLERTYPE_NONE && players < 8)
		players++;
	return players;
}

bool isConnected ( uint8_t playernum)
{
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE;
}
controllerType_e getControllerType ( uint8_t playernum )
{
	return controllers[playernum - 1];
}

bool isPrimaryButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_A) > 0;
			break;
		case CONTROLLERTYPE_LEFT:
			data =  (xmegaGetPressedButtons() & BUTTON_DD) > 0;
			break;
		case CONTROLLERTYPE_RIGHT: 
			data =  (xmegaGetPressedButtons() & BUTTON_X) > 0;
			break;
		default:
			data =  false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressPrimary[playernum - 1];
	}
	prevPressPrimary[playernum - 1] = data;
	return rtn;
}
bool isSecondaryButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_B) > 0;
			break;
		case CONTROLLERTYPE_LEFT:
			data = (xmegaGetPressedButtons() & BUTTON_DL) > 0;
			break;
		case CONTROLLERTYPE_RIGHT: 
			data = (xmegaGetPressedButtons() & BUTTON_A) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressSecondary[playernum - 1];
	}
	prevPressSecondary[playernum - 1] = data;
	return rtn;
}
bool isCoPrimaryButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_X) > 0;
			break;
		case CONTROLLERTYPE_LEFT:
			data = (xmegaGetPressedButtons() & BUTTON_DR) > 0;
			break;
		case CONTROLLERTYPE_RIGHT: 
			data = (xmegaGetPressedButtons() & BUTTON_Y) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressCoPrimary[playernum - 1];
	}
	prevPressCoPrimary[playernum - 1] = data;
	return rtn;
}
bool isCoSecondaryButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_Y) > 0;
			break;
		case CONTROLLERTYPE_LEFT:
			data = (xmegaGetPressedButtons() & BUTTON_DU) > 0;
			break;
		case CONTROLLERTYPE_RIGHT: 
			data = (xmegaGetPressedButtons() & BUTTON_B) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressCoSecondary[playernum - 1];
	}
	prevPressCoSecondary[playernum - 1] = data;
	return rtn;
}

bool hasDedicatedHomeButton ( uint8_t playernum )
{
	return false;
}
bool isHomeButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & (BUTTON_START | BUTTON_SELECT)) == (BUTTON_START | BUTTON_SELECT);
			break;
		case CONTROLLERTYPE_LEFT:
			data = (xmegaGetPressedButtons() & (BUTTON_SELECT | BUTTON_L)) == (BUTTON_SELECT | BUTTON_L);
			break;
		case CONTROLLERTYPE_RIGHT: 
			data = (xmegaGetPressedButtons() & (BUTTON_START | BUTTON_R)) == (BUTTON_START | BUTTON_R);
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressHome[playernum - 1];
		if(rtn){//Da Menubutton ein Teil von Homebutton ist. Muss getzt werden, damit nicht sofort das menu geöffnet wird, wenn homebutton gedrückt wurde
			prevPressMenu[playernum - 1] = true;
			prevPressLMenu[playernum - 1] = true;
			prevPressRMenu[playernum - 1] = true;
		}
	}
	prevPressHome[playernum - 1] = data;
	return rtn;
}

//Shoulder buttons
bool hasShoulderButton( uint8_t playernum ){
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE;
}
bool isShoulderButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & (BUTTON_L | BUTTON_R)) > 0;
			break;
		case CONTROLLERTYPE_LEFT:
			data = (xmegaGetPressedButtons() & BUTTON_L) > 0;
			break;
		case CONTROLLERTYPE_RIGHT: 
			data = (xmegaGetPressedButtons() & BUTTON_R) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressShoulder[playernum - 1];
	}
	prevPressShoulder[playernum - 1] = data;
	return rtn;
}
bool hasLShoulderButton ( uint8_t playernum )
{
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE && controllers[playernum - 1] != CONTROLLERTYPE_RIGHT;
}
bool isLShoulderButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_L) > 0;
			break;
		case CONTROLLERTYPE_LEFT:
			data = (xmegaGetPressedButtons() & BUTTON_L) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressLShoulder[playernum - 1];
	}
	prevPressLShoulder[playernum - 1] = data;
	return rtn;
}
bool hasRShoulderButton ( uint8_t playernum )
{
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE && controllers[playernum - 1] != CONTROLLERTYPE_LEFT;
}
bool isRShoulderButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_R) > 0;
			break;
		case CONTROLLERTYPE_RIGHT: 
			data = (xmegaGetPressedButtons() & BUTTON_R) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressRShoulder[playernum - 1];
	}
	prevPressRShoulder[playernum - 1] = data;
	return rtn;
}


bool isMenuButtonPressed(uint8_t playernum , bool firstTime) 
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & (BUTTON_START | BUTTON_SELECT)) > 0;
			break;
		case CONTROLLERTYPE_LEFT:
			data = (xmegaGetPressedButtons() & BUTTON_SELECT) > 0;
			break;
		case CONTROLLERTYPE_RIGHT: 
			data = (xmegaGetPressedButtons() & BUTTON_START) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressMenu[playernum - 1];
	}
	prevPressMenu[playernum - 1] = data;
	return rtn;
}
bool hasLMenuButton(uint8_t playernum) {
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE && controllers[playernum - 1] != CONTROLLERTYPE_LEFT;
}
bool isLMenuButtonPressed(uint8_t playernum , bool firstTime) 
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_SELECT) > 0;
			break;
		case CONTROLLERTYPE_RIGHT: 
			data = (xmegaGetPressedButtons() & BUTTON_START) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressLMenu[playernum - 1];
	}
	prevPressLMenu[playernum - 1] = data;
	return rtn;
}
bool hasRMenuButton(uint8_t playernum) {
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE && controllers[playernum - 1] != CONTROLLERTYPE_RIGHT;
}
bool isRMenuButtonPressed(uint8_t playernum , bool firstTime) 
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_START) > 0;
			break;
		case CONTROLLERTYPE_LEFT:
			data = (xmegaGetPressedButtons() & BUTTON_SELECT) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressRMenu[playernum - 1];
	}
	prevPressRMenu[playernum - 1] = data;
	return rtn;
}

static uint64_t prevScrollTime[8] = {0,0,0,0,0,0,0,0};
static bool firstSroll[8] = {0,0,0,0,0,0,0,0};
bool shoudScroll(uint8_t playernum, uint16_t ms_first, uint16_t ms_fast){
	if(get_ms_since_boot() - prevScrollTime[playernum - 1] >= ms_first){
		prevScrollTime[playernum - 1] = get_ms_since_boot();
		firstSroll[playernum - 1] = false;
		return true;
	}
	if(get_ms_since_boot() - prevScrollTime[playernum - 1] >= ms_fast && !firstSroll[playernum - 1]){
		prevScrollTime[playernum - 1] = get_ms_since_boot();
		return true;
	}
	return false;
}
bool scrollLInput(uint8_t playernum , uint16_t ms_first, uint16_t ms_fast){
	if(getLInput(playernum,true)){
		firstSroll[playernum - 1] = true;
		prevScrollTime[playernum - 1] = get_ms_since_boot();
		return true;
	}
	if(getLInput(playernum,false)){
		return shoudScroll(playernum, ms_first, ms_fast);
	}
	return false;
}
bool scrollRInput(uint8_t playernum , uint16_t ms_first, uint16_t ms_fast){
	if(getRInput(playernum,true)){
		firstSroll[playernum - 1] = true;
		prevScrollTime[playernum - 1] = get_ms_since_boot();
		return true;
	}
	if(getRInput(playernum,false)){
		return shoudScroll(playernum, ms_first, ms_fast);
	}
	return false;
}
bool scrollUInput(uint8_t playernum , uint16_t ms_first, uint16_t ms_fast){
	if(getUInput(playernum,true)){
		firstSroll[playernum - 1] = true;
		prevScrollTime[playernum - 1] = get_ms_since_boot();
		return true;
	}
	if(getUInput(playernum,false)){
		return shoudScroll(playernum, ms_first, ms_fast);
	}
	return false;
}
bool scrollDInput(uint8_t playernum , uint16_t ms_first, uint16_t ms_fast){
	if(getDInput(playernum,true)){
		firstSroll[playernum - 1] = true;
		prevScrollTime[playernum - 1] = get_ms_since_boot();
		return true;
	}
	if(getDInput(playernum,false)){
		return shoudScroll(playernum, ms_first, ms_fast);
	}
	return false;
}
bool getLInput( uint8_t playernum , bool firstTime)
{
	bool data = getLJoystickXAxis(playernum) < -60;
	bool rtn = data;
	if(firstTime){
		rtn = (data && !prevPressLInput[playernum - 1]) || isDpadLeftButtonPressed(playernum,true);
	}else{
		rtn = data || isDpadLeftButtonPressed(playernum,false);
	}
	prevPressLInput[playernum - 1] = data;
	return rtn;
}
bool getRInput( uint8_t playernum , bool firstTime)
{
	bool data = getLJoystickXAxis(playernum) > 60;
	bool rtn = data;
	if(firstTime){
		rtn = (data && !prevPressRInput[playernum - 1]) || isDpadRightButtonPressed(playernum,true);
	}else{
		rtn = data || isDpadRightButtonPressed(playernum,false);
	}
	prevPressRInput[playernum - 1] = data;
	return rtn;
}
bool getUInput( uint8_t playernum , bool firstTime)
{
	bool data = getLJoystickYAxis(playernum) < -60;
	bool rtn = data;
	if(firstTime){
		rtn = (data && !prevPressUInput[playernum - 1]) || isDpadUpButtonPressed(playernum,true);
	}else{
		rtn = data || isDpadUpButtonPressed(playernum,false);
	}
	prevPressUInput[playernum - 1] = data;
	return rtn;
}
bool getDInput( uint8_t playernum , bool firstTime)
{
	bool data = getLJoystickYAxis(playernum) > 60;
	bool rtn = data;
	if(firstTime){
		rtn = (data && !prevPressDInput[playernum - 1]) || isDpadDownButtonPressed(playernum,true);
	}else{
		rtn = data || isDpadDownButtonPressed(playernum,false);
	}
	prevPressDInput[playernum - 1] = data;
	return rtn;
}

int8_t getLRInput ( uint8_t playernum)
{
	int8_t value = getLJoystickXAxis(playernum);
	if (isDpadLeftButtonPressed(playernum ,false)){
		value = -127;
	}
	if (isDpadRightButtonPressed(playernum ,false)){
		value = 127;
	}
	return value;
}
int8_t getUDInput ( uint8_t playernum)
{
	int8_t value = getLJoystickYAxis(playernum);
	if (isDpadUpButtonPressed(playernum ,false)){
		value = -127;
	}
	if (isDpadDownButtonPressed(playernum ,false)){
		value = 127;
	}
	return value;
}

bool hasDPad ( uint8_t playernum )
{
	return controllers[playernum - 1] == CONTROLLERTYPE_FULL;
}
bool isDpadLeftButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_DL) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressLeft[playernum - 1];
	}
	prevPressLeft[playernum - 1] = data;
	return rtn;
}
bool isDpadRightButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_DR) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressRight[playernum - 1];
	}
	prevPressRight[playernum - 1] = data;
	return rtn;
}
bool isDpadUpButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_DU) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressUp[playernum - 1];
	}
	prevPressUp[playernum - 1] = data;
	return rtn;
}
bool isDpadDownButtonPressed ( uint8_t playernum , bool firstTime)
{
	bool data;
	bool rtn;
	switch(controllers[playernum - 1]){
		case CONTROLLERTYPE_FULL:
			data = (xmegaGetPressedButtons() & BUTTON_DD) > 0;
			break;
		default:
			data = false;
	}
	rtn = data;
	if(firstTime){
		rtn = data && !prevPressDown[playernum - 1];
	}
	prevPressDown[playernum - 1] = data;
	return rtn;
}

bool hasLJoystick ( uint8_t playernum )
{
	return controllers[playernum - 1] != CONTROLLERTYPE_NONE;
}
int8_t getLJoystickXAxis ( uint8_t playernum )
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
int8_t getLJoystickYAxis ( uint8_t playernum )
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
