#include "mainMenu.h"

#include <HLM_battery.h>
#include <HLM_playerInput.h>
//#include "../games/gamesApi/gamesApi.h"
#include <HLM_game.h>

#include <cstdio>

#include <pong/pong.h>
#include <Snake/snake_game.h>
#include <tetris/tetris.h>
#include <sokoban/sokoban.h>
#include "../../platforms/HMatrix/esp32/impl/Websocketserver/server.h"
#include <HLM_graphics.h>
#include "playerIcons.c"

HLM_game games[] = {pong_game, snake_game, tetris_gamedesc, sokoban_game};

#define LEMAGOS_STATE_MAINMENU 0
#define LEMAGOS_STATE_INGAME 1
#define LEMAGOS_STATE_PAUSED 2
#define LEMAGOS_STATE_PLAYERMENU 3
uint8_t state = LEMAGOS_STATE_MAINMENU;

bool prevPressPrimary[8] = {0,0,0,0,0,0,0,0};
bool prevPressCoSecondary[8] = {0,0,0,0,0,0,0,0};
bool prevPressLeft[8] = {0,0,0,0,0,0,0,0};
bool prevPressRight[8] = {0,0,0,0,0,0,0,0};

uint8_t current_brightness = 5;
uint8_t current_sel_game = 0;

void* gameMem;

void DrawMainMenu() {
	HLM_graphics* gfx = get_graphics();
	//server_init();
	switch(state) {
		case LEMAGOS_STATE_MAINMENU: {
			//top bar
			gfx->drawHLine(0, 7, 64, 0x38E7);
			
			//battery
			uint8_t percent = getBatteryPercentage();
			uint16_t color = 0x07E0;
			if (percent < 25)
				color = 0xF800;
			if (percent < 50)
				color = 0xFFE0;
			if (isBatteryCharging())
				color |= 0x001F;
			gfx->drawHLine(50, 1, 12, color);
			gfx->drawHLine(50, 6, 12, color);
			gfx->fillRect(50, 2, 1 + percent/10, 4, color);
			gfx->drawVLine(61, 2, 4, color);
			gfx->drawVLine(62, 3, 2, color);
			
			//players
			gfx->drawHLine(0, 24, 64, 0x38E7);
			for (uint8_t i = 0; i<getPlayerCount(); i++){
				switch(getControllerType(i+1)) {
					case CONTROLLERTYPE_NONE:
						break;
					case CONTROLLERTYPE_FULL:
						gfx->drawImage1Bit(i*8, 24, controller_full_image, 8, 8, 0x0000, CONTROLLER_FULL_COLOR);
						break;
					case CONTROLLERTYPE_RIGHT:
						gfx->drawImage1Bit(i*8, 24, controller_right_image, 8, 8, 0x0000, CONTROLLER_RIGHT_COLOR);
						break;
					case CONTROLLERTYPE_LEFT:
						gfx->drawImage1Bit(i*8, 24, controller_left_image, 8, 8, 0x0000, CONTROLLER_LEFT_COLOR);
						break;
					default:
						gfx->drawImage1Bit(i*8, 24, controller_default_image, 8, 8, 0x0000, CONTROLLER_DEFAULT_COLOR);
						break;
				}
			}
			
			//selected game
			gfx->drawVLine(23, 8, 16, 0x38E7);
			gfx->drawVLine(40, 8, 16, 0x38E7);
			gfx->drawImageRaw(26,10,games[current_sel_game].image);
		
			
			//input handling
			bool draw_special_menu = false;
			for (uint8_t i = 0; i < 8; i++) {
				if (isPrimaryButtonPressed(i+1) && !prevPressPrimary[i]) {
					gfx->clear();
					if (state != LEMAGOS_STATE_INGAME) { //two players starting the game in the same time would be an issue
						state = LEMAGOS_STATE_INGAME;
						gameMem = games[current_sel_game].setupFunction();
					}
				}
				if (isCoSecondaryButtonPressed(i+1) && !prevPressCoSecondary[i]) {
					//cycleInputMethods();
				}
				//gfx->setTextColor(GRAPHICS_COLOR_WHITE);
				
				if (isShoulderButtonPressed(i+1))
					draw_special_menu = true;
				if (getLRInput(i+1) < -0x40 && !prevPressLeft[i]) { //left
					if (isShoulderButtonPressed(i+1)) {
						if (current_brightness > 1)
							current_brightness--;
						gfx->setBrightness(current_brightness);
					}
					else
					{
						if (current_sel_game != 0)
							current_sel_game--;
						else
							current_sel_game = sizeof(games)/sizeof(games[0]) - 1;
					}
				}
				if (getLRInput(i+1) > 0x40 && !prevPressRight[i]) { //right
					if (isShoulderButtonPressed(i+1)) {
						if (current_brightness < 100) {
							current_brightness++;
						}
						gfx->setBrightness(current_brightness);
					}
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
				gfx->drawText(0, 0, batstring, 0xFFFF);
				sprintf(batstring, "%d", getBatteryPercentage());
				gfx->drawText(32, 0, batstring, 0xFFFF);
			} else {
				gfx->drawText(0, 0, games[current_sel_game].name, 0xFFFF);
			}
		}
		break;
		case LEMAGOS_STATE_INGAME: {
			if (isHomeButtonPressed(1)){
				gfx->clear();
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
