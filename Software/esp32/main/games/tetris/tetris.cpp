#include "tetris.h"
#include "tetris_game.h"
#include "tetris_draw.h"
#include <stdlib.h>
#include "../../graphics.h"
#include "../../playerInput/playerInput.h"

typedef struct tetrismem_t {
	MatrixPanel_I2S_DMA* gfx;
	void (* drawFuncField)(uint8_t x, uint8_t y, uint16_t color, MatrixPanel_I2S_DMA* gfx);
	void (* drawFuncHUD)(class tetris_game *gameplay, MatrixPanel_I2S_DMA* gfx);
	class tetris_game *gameplay;
} pongmem_t;

#define TETRISMEM ((pongmem_t*)gamemem)

static const char *TAG = "Tetris";

const LaLeMaGame tetris_gamedesc = {
	"Tetris",
	*tetrisgame_setup,
	*tetrisgame_loop,
	*tetrisgame_free
};

void tetrisgame_saveHighscore(uint8_t index, char name[10], uint32_t score){
	
}
void tetrisgame_readHighscore(uint8_t index, char name[10], uint32_t score){
	
}

void* tetrisgame_setup(){
	tetrismem_t *pointer = (tetrismem_t*)malloc(sizeof(tetrismem_t));
	pointer->gfx = getGraphics();
	pointer->drawFuncField = drawFuncField1p1;
	pointer->drawFuncHUD = drawFuncHUD1p1;
	pointer->gameplay = new class tetris_game();
	pointer->gameplay->saveHighscoreFunc = tetrisgame_saveHighscore;
	pointer->gameplay->readHighscoreFunc = tetrisgame_readHighscore;
	pointer->gameplay->init();
	return pointer;
}
void tetrisgame_loop(void* gamemem){
	TETRISMEM->gameplay->tick();

	//Input handling
	TETRISMEM->gameplay->left(getLRInput(1) < -60);
	TETRISMEM->gameplay->right(getLRInput(1) > 60);
	TETRISMEM->gameplay->down(getUDInput(1) > 60);
	TETRISMEM->gameplay->pause(isMenuButtonPressed(1));
	TETRISMEM->gameplay->rotateR(isPrimaryButtonPressed(1));
	TETRISMEM->gameplay->rotateL(isSecondaryButtonPressed(1));
	if (getRJoystickXAxis(1) > 60)
		TETRISMEM->gameplay->rotate(1);
	if (getRJoystickXAxis(1) < -60)
		TETRISMEM->gameplay->rotate(3);
	if (getRJoystickYAxis(1) > 60)
		TETRISMEM->gameplay->rotate(2);
	if (getRJoystickYAxis(1) < -60)
		TETRISMEM->gameplay->rotate(0);

	//Draw
	TETRISMEM->drawFuncHUD(TETRISMEM->gameplay, TETRISMEM->gfx);
	if (TETRISMEM->gameplay->game_state == GAME_STATE_PLAY) {
		for(uint8_t y = 0; y < 16; y++) {
			TETRISMEM->drawFuncField(0,y, 0x2222, TETRISMEM->gfx);
			TETRISMEM->drawFuncField(11,y, 0x2222, TETRISMEM->gfx);
		}
		for(uint8_t x = 0; x < 10; x++) {
			for (uint8_t y = 0; y < 16; y++) {
				switch(TETRISMEM->gameplay->field[x][y]) {
					case 1: TETRISMEM->drawFuncField(1+x, y, 0xF800, TETRISMEM->gfx); break;
					case 2: TETRISMEM->drawFuncField(1+x, y, 0x07E0, TETRISMEM->gfx); break;
					case 3: TETRISMEM->drawFuncField(1+x, y, 0x001F, TETRISMEM->gfx); break;
					case 4: TETRISMEM->drawFuncField(1+x, y, 0xFFE0, TETRISMEM->gfx); break;
					case 5: TETRISMEM->drawFuncField(1+x, y, 0xF81F, TETRISMEM->gfx); break;
					case 6: TETRISMEM->drawFuncField(1+x, y, 0x07FF, TETRISMEM->gfx); break;
					case 7: TETRISMEM->drawFuncField(1+x, y, 0xFFFF, TETRISMEM->gfx); break;
					case 0x80: TETRISMEM->drawFuncField(1+x, y, 0x8710, TETRISMEM->gfx); break;
				}
			}
		}
		for (uint8_t _y = 0; _y < 4; _y++) {
			for (uint8_t _x = 0; _x < 4; _x++) {
				if (TETRISMEM->gameplay->currenty+_y < 0)
					continue;
				switch((*TETRISMEM->gameplay->currentblock)[_y][_x]) {
					case 1: TETRISMEM->drawFuncField(1+TETRISMEM->gameplay->currentx+_x,TETRISMEM->gameplay->currenty+_y, 0xF800, TETRISMEM->gfx); break;
					case 2: TETRISMEM->drawFuncField(1+TETRISMEM->gameplay->currentx+_x,TETRISMEM->gameplay->currenty+_y, 0x07E0, TETRISMEM->gfx); break;
					case 3: TETRISMEM->drawFuncField(1+TETRISMEM->gameplay->currentx+_x,TETRISMEM->gameplay->currenty+_y, 0x001F, TETRISMEM->gfx); break;
					case 4: TETRISMEM->drawFuncField(1+TETRISMEM->gameplay->currentx+_x,TETRISMEM->gameplay->currenty+_y, 0xFFE0, TETRISMEM->gfx); break;
					case 5: TETRISMEM->drawFuncField(1+TETRISMEM->gameplay->currentx+_x,TETRISMEM->gameplay->currenty+_y, 0xF81F, TETRISMEM->gfx); break;
					case 6: TETRISMEM->drawFuncField(1+TETRISMEM->gameplay->currentx+_x,TETRISMEM->gameplay->currenty+_y, 0x07FF, TETRISMEM->gfx); break;
					case 7: TETRISMEM->drawFuncField(1+TETRISMEM->gameplay->currentx+_x,TETRISMEM->gameplay->currenty+_y, 0xFFFF, TETRISMEM->gfx); break;
				}
			}
		}
		for (uint8_t _y = 0; _y < 4; _y++) {
			for (uint8_t _x = 0; _x < 4; _x++) {
				switch((*TETRISMEM->gameplay->nextblock)[_y][_x]) {
					case 1: TETRISMEM->drawFuncField(12+_x, _y, 0xF800, TETRISMEM->gfx); break;
					case 2: TETRISMEM->drawFuncField(12+_x, _y, 0x07E0, TETRISMEM->gfx); break;
					case 3: TETRISMEM->drawFuncField(12+_x, _y, 0x001F, TETRISMEM->gfx); break;
					case 4: TETRISMEM->drawFuncField(12+_x, _y, 0xFFE0, TETRISMEM->gfx); break;
					case 5: TETRISMEM->drawFuncField(12+_x, _y, 0xF81F, TETRISMEM->gfx); break;
					case 6: TETRISMEM->drawFuncField(12+_x, _y, 0x07FF, TETRISMEM->gfx); break;
					case 7: TETRISMEM->drawFuncField(12+_x, _y, 0xFFFF, TETRISMEM->gfx); break;
				}
			}
		}
	}
}
void tetrisgame_free(void* gamemem){
	TETRISMEM->gameplay->free();
	TETRISMEM->gameplay->~tetris_game();
	free(gamemem);
}
