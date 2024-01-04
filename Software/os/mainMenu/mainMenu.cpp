#include "mainMenu.h"

#include <HLM_battery.h>
#include <HLM_playerInput.h>
#include <HLM_game.h>

#include <cstdio>

#include <pong/pong.h>
#include <Snake/snake_game.h>
#include <tetris/tetris.h>
#include <sokoban/sokoban.h>
#include <tictactoe/tictactoe.h>

#include "../settingsMenu/settingsMenu.h"
#include "../accounts/accounts.h"

#include <HLM_graphics.h>
#include "playerIcons.c"

#include <defaultInputEvents.h>

HLM_game games[] = {pong_game, snake_game, tetris_gamedesc, sokoban_game, tictactoe_game};
//HLM_game games[] = {pong_game, snake_game, tetris_gamedesc, sokoban_game};

#define LEMAGOS_STATE_MAINMENU 0
#define LEMAGOS_STATE_INGAME 1
#define LEMAGOS_STATE_SETTINGSMENU 2
uint8_t state = LEMAGOS_STATE_MAINMENU;

uint8_t current_sel_game = 0;
uint8_t current_account = 0;

void* gameMem;

void DrawMainMenu() {
	HLM_graphics* gfx = get_graphics();
	Accounts* acc = get_accounts();
	
	switch(state) {
		case LEMAGOS_STATE_MAINMENU: {
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
			gfx->drawHLine(0, 8, 64, 0x38E7);
			gfx->drawHLine(0, 23, 64, 0x38E7);
			gfx->drawVLine(24, 8, 16, 0x38E7);
			gfx->drawVLine(39, 8, 16, 0x38E7);
			gfx->drawImageRaw(26,10,games[current_sel_game].image);
			gfx->drawText(0, 0, games[current_sel_game].name, 0xFFFF);

			char buffer[10];
			acc->getName(buffer);
			gfx->drawText(20, 24, buffer, 0xFFFF);
			
			for (uint8_t i = 1; i <= 8; i++) {
	
				if (gotPrimaryButtonPressed(i, false)) {
					gfx->clear();
					if (state != LEMAGOS_STATE_INGAME) { //two players starting the game in the same time would be an issue
						state = LEMAGOS_STATE_INGAME;
						gameMem = games[current_sel_game].setupFunction();
					}
				}
				if(gotDownButtonPressed(i,false) && acc->active_account < ACOOUNT_NUM - 1){
					acc->active_account++;
				}
				if(gotUpButtonPressed(i,false) && acc->active_account > 0){
					acc->active_account--;
				}
				if (gotLeftButtonPressed(i, false)) { //left	
					if (current_sel_game != 0)
						current_sel_game--;
					else
						current_sel_game = sizeof(games)/sizeof(games[0]) - 1;
				}
				if (gotRightButtonPressed(i, false)) { //right
					if (current_sel_game < sizeof(games)/sizeof(games[0]) - 1)
						current_sel_game++;
					else
						current_sel_game = 0;
				}
				if(gotMenuButtonPressed(i, false)){
					gfx->clear();
					setting_menu_init();
					state = LEMAGOS_STATE_SETTINGSMENU;
				}
			}
		}
		break;
		case LEMAGOS_STATE_INGAME: {
			if (gotHomeButtonPressed(1, false)){
				gfx->clear();
				state = LEMAGOS_STATE_MAINMENU;
				games[current_sel_game].freeFunction(gameMem);
			} else {
				games[current_sel_game].loopFunction(gameMem);
			}
		}
		break;
		case LEMAGOS_STATE_SETTINGSMENU: {
			DrawSettingsMenu();
			for (uint8_t i = 1; i <= 8; i++) {
				if(gotMenuButtonPressed(i, false)){
					gfx->clear();
					settings_menu_free();
					state = LEMAGOS_STATE_MAINMENU;
				}
			}
		}
		break;
	}
}
