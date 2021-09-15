#include "mainMenu.h"

#include "../powerControl/powerControl.h"
#include "../playerInput/playerInput.h"
#include "../games/gamesApi/gamesApi.h"
#include "../games/pong/pong.h"

LaLeMaGame games[] = {pong_game};

#define LEMAGOS_STATE_MAINMENU 0
#define LEMAGOS_STATE_INGAME 1
#define LEMAGOS_STATE_PAUSED 2
uint8_t state = LEMAGOS_STATE_MAINMENU;

void* gameMem;

void DrawMainMenu ( MatrixPanel_I2S_DMA* gfx )
{
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
			
			//selected game
			gfx->drawFastVLine(23, 8, 16, 0x38E7);
			gfx->drawFastVLine(40, 8, 16, 0x38E7);
			
			gfx->drawRect(25, 9, 14, 14, 0x38E7);
			
			//input handling
			if (isPrimaryButtonPressed(1)){
				gfx->clearScreen();
				state = LEMAGOS_STATE_INGAME;
				gameMem = games[0].setupFunction();
			}
		}
		break;
		case LEMAGOS_STATE_INGAME: {
			if (isHomeButtonPressed(1)){
				gfx->clearScreen();
				state = LEMAGOS_STATE_MAINMENU;
				games[0].freeFunction(gameMem);
			} else {
				games[0].loopFunction(gameMem);
			}
		}
		break;
	}
}
