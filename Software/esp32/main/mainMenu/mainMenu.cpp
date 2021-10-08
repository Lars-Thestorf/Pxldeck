#include "mainMenu.h"

#include "../powerControl/powerControl.h"
#include "../playerInput/playerInput.h"
#include "../games/gamesApi/gamesApi.h"
#include "../games/pong/pong.h"
#include "../games/tetris/tetris.h"

#include "../graphics.h"

#include "../playerIcons.c"

LaLeMaGame games[] = {pong_game, tetris_gamedesc};

#define LEMAGOS_STATE_MAINMENU 0
#define LEMAGOS_STATE_INGAME 1
#define LEMAGOS_STATE_PAUSED 2
#define LEMAGOS_STATE_PLAYERMENU 3
uint8_t state = LEMAGOS_STATE_MAINMENU;

bool prevPressPrimary[8] = {0,0,0,0,0,0,0,0};
bool prevPressCoSecondary[8] = {0,0,0,0,0,0,0,0};
bool prevPressLeft[8] = {0,0,0,0,0,0,0,0};
bool prevPressRight[8] = {0,0,0,0,0,0,0,0};

uint8_t current_brightness = 20;
uint8_t current_sel_game = 0;

void* gameMem;

void DrawMainMenu() {
	MatrixPanel_I2S_DMA* gfx = getGraphics();
	switch(state) {
		case LEMAGOS_STATE_MAINMENU: {
			//top bar
			gfx->drawFastHLine(0, 7, 64, 0x38E7);
			
			//battery
			uint8_t percent = getBatteryPercentage();
			uint16_t color = 0x07E0;
			if (percent < 25)
				color = 0xF800;
			if (percent < 50)
				color = 0xFFE0;
			if (isDeviceCharging())
				color |= 0x001F;
			gfx->drawFastHLine(50, 1, 12, color);
			gfx->drawFastHLine(50, 6, 12, color);
			gfx->fillRect(50, 2, 1 + percent/10, 4, color);
			gfx->drawFastVLine(61, 2, 4, color);
			gfx->drawFastVLine(62, 3, 2, color);
			
			//players
			gfx->drawFastHLine(0, 24, 64, 0x38E7);
			for (uint8_t i = 0; i<getPlayerCount(); i++){
				switch(getControllerType(i+1)) {
					case CONTROLLERTYPE_NONE:
						break;
					case CONTROLLERTYPE_FULL:
						drawImage1bit(i*8, 24, controller_full_image, 8, 8, 0x0000, CONTROLLER_FULL_COLOR);
						break;
					case CONTROLLERTYPE_RIGHT:
						drawImage1bit(i*8, 24, controller_right_image, 8, 8, 0x0000, CONTROLLER_RIGHT_COLOR);
						break;
					case CONTROLLERTYPE_LEFT:
						drawImage1bit(i*8, 24, controller_left_image, 8, 8, 0x0000, CONTROLLER_LEFT_COLOR);
						break;
					default:
						drawImage1bit(i*8, 24, controller_default_image, 8, 8, 0x0000, CONTROLLER_DEFAULT_COLOR);
						break;
				}
			}
			
			//selected game
			gfx->drawFastVLine(23, 8, 16, 0x38E7);
			gfx->drawFastVLine(40, 8, 16, 0x38E7);
			
			gfx->drawRect(25, 9, 14, 14, 0x38E7);
			
			//Debug sticks
			gfx->drawLine(6, 15, 6 + getLRInput(1) / 30, 15 + getUDInput(1) / 30, 0xFFFF);
			gfx->drawLine(57, 15, 57 + getRJoystickXAxis(1) / 30, 15 + getRJoystickYAxis(1) / 30, 0xFFFF);
			
			//input handling
			bool draw_special_menu = false;
			for (uint8_t i = 0; i < 8; i++) {
				if (isPrimaryButtonPressed(i+1) && !prevPressPrimary[i]) {
					gfx->clearScreen();
					if (state != LEMAGOS_STATE_INGAME) { //two players starting the game in the same time would be an issue
						state = LEMAGOS_STATE_INGAME;
						gameMem = games[current_sel_game].setupFunction();
					}
				}
				if (isCoSecondaryButtonPressed(i+1) && !prevPressCoSecondary[i]) {
					cycleInputMethods();
				}
				gfx->setTextColor(GRAPHICS_COLOR_WHITE);
				
				if (isShoulderButtonPressed(i+1))
					draw_special_menu = true;
				if (getLRInput(i+1) < -0x40 && !prevPressLeft[i]) { //left
					if (isShoulderButtonPressed(i+1))
						gfx->setPanelBrightness(--current_brightness);
					else {
						if (current_sel_game != 0)
							current_sel_game--;
						else
							current_sel_game = sizeof(games)/sizeof(games[0]) - 1;
					}
				}
				if (getLRInput(i+1) > 0x40 && !prevPressRight[i]) { //right
					if (isShoulderButtonPressed(i+1))
						gfx->setPanelBrightness(++current_brightness);
					else {
						if (current_sel_game < sizeof(games)/sizeof(games[0]) - 1)
							current_sel_game++;
						else
							current_sel_game = 0;
					}
				}
				 
				prevPressPrimary[i] = isPrimaryButtonPressed(i+1);
				prevPressCoSecondary[i] = isCoPrimaryButtonPressed(i+1);
				prevPressLeft[i] = getLRInput(i+1) < -0x40;
				prevPressRight[i] = getLRInput(i+1) > 0x40;
			}
			if (draw_special_menu) {
				char batstring[10];
				sprintf(batstring, "*%d", current_brightness);
				drawString(0,0,batstring);
				sprintf(batstring, "%d", getBatteryPercentage());
				drawString(32,0,batstring);
			} else {
				drawString(0,0,games[current_sel_game].name);
			}
		}
		break;
		case LEMAGOS_STATE_INGAME: {
			if (isHomeButtonPressed(1)){
				gfx->clearScreen();
				state = LEMAGOS_STATE_MAINMENU;
				games[current_sel_game].freeFunction(gameMem);
			} else {
				games[current_sel_game].loopFunction(gameMem);
			}
		}
		break;
		case LEMAGOS_STATE_PLAYERMENU: {
			
		}
		break;
	}
}
