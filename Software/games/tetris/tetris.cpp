#include "tetris.h"
#include "tetris_game.h"
#include "tetris_draw.h"
#include <stdlib.h>
#include <cstdio>
#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include <HLM_storage.h>
#include <defaultInputEvents.h>

typedef struct tetrismem_t {
	HLM_graphics* gfx;
	void (* drawFuncField)(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx);
	void (* drawFuncHUD)(class tetris_game *gameplay, HLM_graphics* gfx);
	class tetris_game *gameplay;
} pongmem_t;

#define TETRISMEM ((pongmem_t*)gamemem)

const uint8_t icon[] = {0x00, 0x01, 0x20, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0x00, 0x00, 0xC7, 0x48, 0x00, 0x00, 0x00, 0x00, 0xAE, 0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0xC7, 0x48, 0xC7, 0x48, 0xAE, 0xBC, 0xAE, 0xBC, 0xAE, 0xBC, 0xF7, 0xC8, 0x00, 0x00, 0xE3, 0xF7, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0xC7, 0x48, 0xF7, 0xC8, 0x49, 0xB8, 0x49, 0xB8, 0xF7, 0xC8, 0xF7, 0xC8, 0x00, 0x00, 0xE3, 0xF7, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0xF7, 0xC8, 0xF7, 0xC8, 0xF7, 0xC8, 0x49, 0xB8, 0x49, 0xB8, 0xF7, 0xC8, 0xE3, 0xF7, 0xE3, 0xF7, 0x00, 0x00, 0x65, 0x8A};

const HLM_game tetris_gamedesc = {
	"Tetris",
	icon,
	*tetrisgame_setup,
	*tetrisgame_loop,
	*tetrisgame_free
};

void tetrisgame_saveHighscore(uint8_t index, tetris_highscore_entry_t highscore_entry){
	char tetris_storage_key[12];
	snprintf(tetris_storage_key, 12, "TetrisS%d", index) ;
	HLM_storage_write32(tetris_storage_key, highscore_entry.score);
	printf("write tetris Score: %d\n", highscore_entry.score);	
}
tetris_highscore_entry_t tetrisgame_readHighscore(uint8_t index){
	tetris_highscore_entry_t ret;
	ret.name[0] = '\0';
	ret.score = 0;
	char tetris_storage_key[12];
	snprintf(tetris_storage_key, 12, "TetrisS%d", index) ;
	if (HLM_storage_exists32(tetris_storage_key)) {
		ret.score = HLM_storage_read32(tetris_storage_key);
		printf("read tetris Score: %d\n", ret.score);
	}
	return ret;
}

void* tetrisgame_setup(){
	tetrismem_t *pointer = (tetrismem_t*)malloc(sizeof(tetrismem_t));
	pointer->gfx = get_graphics();
	pointer->drawFuncField = drawFuncField1p1;
	pointer->drawFuncHUD = drawFuncHUD1p1;
	pointer->gameplay = new class tetris_game();
	pointer->gameplay->saveHighscoreFunc = tetrisgame_saveHighscore;
	pointer->gameplay->readHighscoreFunc = tetrisgame_readHighscore;
	for(int i = 0;i < SCORE_LENGTH;i++){
		pointer->gameplay->highscores[i] = tetrisgame_readHighscore(i);
	}
	pointer->gameplay->init();
	return pointer;
}
void tetrisgame_loop(void* gamemem){
	TETRISMEM->gameplay->tick();

	//Input handling
	TETRISMEM->gameplay->left(gotLeftButtonPressed(1, true));
	TETRISMEM->gameplay->right(gotRightButtonPressed(1, true));
	TETRISMEM->gameplay->down(gotDownButtonPressed(1, true));
	TETRISMEM->gameplay->up(gotUpButtonPressed(1, true));
	TETRISMEM->gameplay->pause(gotMenuButtonPressed(1, false));
	TETRISMEM->gameplay->rotateR(gotPrimaryButtonPressed(1, false));
	TETRISMEM->gameplay->rotateL(gotSecondaryButtonPressed(1, false));

	TETRISMEM->gameplay->holdingDown = getUDInput(1) > 60;
	TETRISMEM->gameplay->holdingLeft = getLRInput(1) < -60;
	TETRISMEM->gameplay->holdingRight = getLRInput(1) > 60;
	
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
