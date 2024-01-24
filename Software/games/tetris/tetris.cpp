#include "tetris.h"
#include "tetris_game.h"
#include "tetris_draw.h"
#include <stdlib.h>
#include <cstdio>
#include <HLM_playerInput.h>
#include <HLM_storage.h>
#include <defaultInputEvents.h>
#include "../../os/accounts/accounts.h"

void handle_highscore(tetris_game *game);
void tetrisgame_saveHighscore(uint8_t index, tetris_highscore_entry_t highscore_entry);
tetris_highscore_entry_t tetrisgame_readHighscore(uint8_t index);
bool isScoreHighscore (uint32_t score);
bool addToHighscoreList (tetris_highscore_entry_t highscore_entry);
void buildHighscoreList();

tetrismem_t *pointer_tetris;
const uint8_t icon[] = {0x00, 0x01, 0x20, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0x00, 0x00, 0xC7, 0x48, 0x00, 0x00, 0x00, 0x00, 0xAE, 0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0xC7, 0x48, 0xC7, 0x48, 0xAE, 0xBC, 0xAE, 0xBC, 0xAE, 0xBC, 0xF7, 0xC8, 0x00, 0x00, 0xE3, 0xF7, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0xC7, 0x48, 0xF7, 0xC8, 0x49, 0xB8, 0x49, 0xB8, 0xF7, 0xC8, 0xF7, 0xC8, 0x00, 0x00, 0xE3, 0xF7, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0xC9, 0x86, 0xF7, 0xC8, 0xF7, 0xC8, 0xF7, 0xC8, 0x49, 0xB8, 0x49, 0xB8, 0xF7, 0xC8, 0xE3, 0xF7, 0xE3, 0xF7, 0x00, 0x00, 0x65, 0x8A};

const HLM_game tetris_gamedesc = {
	"Tetris",
	icon,
	*tetrisgame_setup,
	*tetrisgame_loop,
	*tetrisgame_free
};

void* tetrisgame_setup(){
	pointer_tetris = (tetrismem_t*)malloc(sizeof(tetrismem_t));
	pointer_tetris->gfx = get_graphics();
	pointer_tetris->highscoreIndex = 0;
	pointer_tetris->playerCount = getPlayerCount();
	if(pointer_tetris->playerCount >= 1){
		switch (getControllerType(0)){
		case CONTROLLERTYPE_FULL:
			pointer_tetris->drawFuncField[0] = drawFuncField1p1;
			break;
		case CONTROLLERTYPE_LEFT:
			pointer_tetris->drawFuncField[0] = drawFuncField2p1;
			break;
		case CONTROLLERTYPE_RIGHT:
			pointer_tetris->drawFuncField[0] = drawFuncField2p2;
			break;
		default:
			pointer_tetris->drawFuncField[0] = drawFuncField1p1;
			break;
		}
		pointer_tetris->drawFuncHUD[0] = drawFuncHUD1p1;
		pointer_tetris->gameplay[0] = new class tetris_game();
		pointer_tetris->gameplay[0]->init();
		pointer_tetris->gameplay[0]->game_end = handle_highscore;

		Accounts *acc = get_accounts();
		acc->getName(pointer_tetris->name[0]);
	}
	if(pointer_tetris->playerCount >= 2){
		pointer_tetris->drawFuncField[0] = drawFuncField2p1;
		pointer_tetris->drawFuncField[1] = drawFuncField2p2;
		pointer_tetris->drawFuncHUD[0] = drawFuncHUD2p1;
		pointer_tetris->drawFuncHUD[1] = drawFuncHUD2p2;
		pointer_tetris->gameplay[1] = new class tetris_game();
		pointer_tetris->gameplay[1]->init();
		pointer_tetris->gameplay[1]->game_end = handle_highscore;

		Accounts *acc = get_accounts();
		acc->getName(pointer_tetris->name[1]);
	}
	for(int i = 0;i < SCORE_LENGTH;i++){
		pointer_tetris->highscores[i] = tetrisgame_readHighscore(i);
	}
	return pointer_tetris;
}
void tetrisgame_loop(void* gamemem){
	for(uint8_t i= 0;i < pointer_tetris->playerCount;i++){
		pointer_tetris->gameplay[i]->tick();

		//Input handling
		pointer_tetris->gameplay[i]->left(gotLeftButtonPressed(i + 1, true));
		pointer_tetris->gameplay[i]->right(gotRightButtonPressed(i + 1, true));
		pointer_tetris->gameplay[i]->pause(gotMenuButtonPressed(i + 1, false));
		pointer_tetris->gameplay[i]->rotateR(gotPrimaryButtonPressed(i + 1, false));
		pointer_tetris->gameplay[i]->rotateL(gotSecondaryButtonPressed(i + 1, false));

		pointer_tetris->gameplay[i]->holdingDown = getUDInput(i + 1) > 60;
		pointer_tetris->gameplay[i]->holdingLeft = getLRInput(i + 1) < -60;
		pointer_tetris->gameplay[i]->holdingRight = getLRInput(i + 1) > 60;
		
		if (getRJoystickXAxis(i + 1) > 60)
			pointer_tetris->gameplay[i]->rotate(1);
		if (getRJoystickXAxis(i + 1) < -60)
			pointer_tetris->gameplay[i]->rotate(3);
		if (getRJoystickYAxis(i + 1) > 60)
			pointer_tetris->gameplay[i]->rotate(2);
		if (getRJoystickYAxis(i + 1) < -60)
			pointer_tetris->gameplay[i]->rotate(0);
		if (gotDownButtonPressed(i,true) && pointer_tetris->gameplay[i]->game_state == GAME_STATE_MENU){
			if (pointer_tetris->highscoreIndex < SCORE_LENGTH - 1)
				pointer_tetris->highscoreIndex++;
		}
		if (gotUpButtonPressed(i,true) && pointer_tetris->gameplay[i]->game_state == GAME_STATE_MENU){
			if (pointer_tetris->highscoreIndex > 0)
				pointer_tetris->highscoreIndex--;
		}

		//Draw
		pointer_tetris->drawFuncHUD[i](pointer_tetris, pointer_tetris->gfx);
		if (pointer_tetris->gameplay[i]->game_state == GAME_STATE_PLAY) {
			for(uint8_t x = 0; x < 10; x++) {
				for (uint8_t y = 0; y < 16; y++) {
					switch(pointer_tetris->gameplay[i]->field[x][y]) {
						case 1: pointer_tetris->drawFuncField[i](x, y, 0xF800, pointer_tetris->gfx); break;
						case 2: pointer_tetris->drawFuncField[i](x, y, 0x07E0, pointer_tetris->gfx); break;
						case 3: pointer_tetris->drawFuncField[i](x, y, 0x001F, pointer_tetris->gfx); break;
						case 4: pointer_tetris->drawFuncField[i](x, y, 0xFFE0, pointer_tetris->gfx); break;
						case 5: pointer_tetris->drawFuncField[i](x, y, 0xF81F, pointer_tetris->gfx); break;
						case 6: pointer_tetris->drawFuncField[i](x, y, 0x07FF, pointer_tetris->gfx); break;
						case 7: pointer_tetris->drawFuncField[i](x, y, 0xFFFF, pointer_tetris->gfx); break;
						case 0x80: pointer_tetris->drawFuncField[i](x, y, 0x8710, pointer_tetris->gfx); break;
					}
				}
			}
			for (uint8_t _y = 0; _y < 4; _y++) {
				for (uint8_t _x = 0; _x < 4; _x++) {
					if (pointer_tetris->gameplay[i]->currenty+_y < 0)
						continue;
					switch((*pointer_tetris->gameplay[i]->currentblock)[_y][_x]) {
						case 1: pointer_tetris->drawFuncField[i](pointer_tetris->gameplay[i]->currentx+_x,pointer_tetris->gameplay[i]->currenty+_y, 0xF800, pointer_tetris->gfx); break;
						case 2: pointer_tetris->drawFuncField[i](pointer_tetris->gameplay[i]->currentx+_x,pointer_tetris->gameplay[i]->currenty+_y, 0x07E0, pointer_tetris->gfx); break;
						case 3: pointer_tetris->drawFuncField[i](pointer_tetris->gameplay[i]->currentx+_x,pointer_tetris->gameplay[i]->currenty+_y, 0x001F, pointer_tetris->gfx); break;
						case 4: pointer_tetris->drawFuncField[i](pointer_tetris->gameplay[i]->currentx+_x,pointer_tetris->gameplay[i]->currenty+_y, 0xFFE0, pointer_tetris->gfx); break;
						case 5: pointer_tetris->drawFuncField[i](pointer_tetris->gameplay[i]->currentx+_x,pointer_tetris->gameplay[i]->currenty+_y, 0xF81F, pointer_tetris->gfx); break;
						case 6: pointer_tetris->drawFuncField[i](pointer_tetris->gameplay[i]->currentx+_x,pointer_tetris->gameplay[i]->currenty+_y, 0x07FF, pointer_tetris->gfx); break;
						case 7: pointer_tetris->drawFuncField[i](pointer_tetris->gameplay[i]->currentx+_x,pointer_tetris->gameplay[i]->currenty+_y, 0xFFFF, pointer_tetris->gfx); break;
					}
				}
			}
			for (uint8_t _y = 0; _y < 4; _y++) {
				for (uint8_t _x = 0; _x < 4; _x++) {
					switch((*pointer_tetris->gameplay[i]->nextblock)[_y][_x]) {
						case 1: pointer_tetris->drawFuncField[i](11+_x, _y, 0xF800, pointer_tetris->gfx); break;
						case 2: pointer_tetris->drawFuncField[i](11+_x, _y, 0x07E0, pointer_tetris->gfx); break;
						case 3: pointer_tetris->drawFuncField[i](11+_x, _y, 0x001F, pointer_tetris->gfx); break;
						case 4: pointer_tetris->drawFuncField[i](11+_x, _y, 0xFFE0, pointer_tetris->gfx); break;
						case 5: pointer_tetris->drawFuncField[i](11+_x, _y, 0xF81F, pointer_tetris->gfx); break;
						case 6: pointer_tetris->drawFuncField[i](11+_x, _y, 0x07FF, pointer_tetris->gfx); break;
						case 7: pointer_tetris->drawFuncField[i](11+_x, _y, 0xFFFF, pointer_tetris->gfx); break;
					}
				}
			}
		}
	}
}
void tetrisgame_free(void* gamemem){
	for(uint8_t i= 0;i < pointer_tetris->playerCount;i++){
		pointer_tetris->gameplay[i]->free();
		pointer_tetris->gameplay[i]->~tetris_game();
	}
	free(gamemem);
}

void handle_highscore(tetris_game *game){
	if (isScoreHighscore(game->score)){
		pointer_tetris->highscores[SCORE_LENGTH - 1].score = game->score; 
		snprintf(pointer_tetris->highscores[SCORE_LENGTH - 1].name,10,"%s",pointer_tetris->name[0]); 
		buildHighscoreList();
	}
}
void tetrisgame_saveHighscore(uint8_t index, tetris_highscore_entry_t highscore_entry){
	char tetris_storage_key[12];
	snprintf(tetris_storage_key, 12, "TetrisS%d", index) ;
	HLM_storage_write32(tetris_storage_key, highscore_entry.score);
	snprintf(tetris_storage_key, 12, "TetrisN%d", index) ;
	HLM_storage_write_str(tetris_storage_key, highscore_entry.name);
	printf("write tetris Score: %ld\n", highscore_entry.score);	
	printf("write tetris name: %s\n", highscore_entry.name);	
}
tetris_highscore_entry_t tetrisgame_readHighscore(uint8_t index){
	tetris_highscore_entry_t ret;
	ret.name[0] = '\0';
	ret.score = 0;
	size_t size = 10;
	char tetris_storage_key[12];
	snprintf(tetris_storage_key, 12, "TetrisS%d", index) ;
	if (HLM_storage_exists32(tetris_storage_key)) {
		ret.score = HLM_storage_read32(tetris_storage_key);
		printf("read tetris Score: %ld\n", ret.score);
	}
	snprintf(tetris_storage_key, 12, "TetrisN%d", index);
	HLM_storage_read_str(tetris_storage_key, ret.name, &size);
	printf("read tetris Name: %s\n", ret.name);	
	return ret;
}

bool isScoreHighscore ( uint32_t score )
{
	return score > pointer_tetris->highscores[SCORE_LENGTH - 1].score;
}
bool addToHighscoreList (tetris_highscore_entry_t highscore_entry)
{
	if (!isScoreHighscore(highscore_entry.score))
		return false;

	pointer_tetris->highscores[SCORE_LENGTH - 1] = highscore_entry; 

	buildHighscoreList(); // Rebuild highscore list (sorting)
	return true;
}

void buildHighscoreList()
{
	uint8_t i, j, max_idx;
 
    for (i = 0; i < SCORE_LENGTH - 1; i++) {
        max_idx = i;
        for (j = i + 1; j < SCORE_LENGTH; j++)
            if (pointer_tetris->highscores[j].score > pointer_tetris->highscores[max_idx].score){
				 max_idx = j;
			}
		uint32_t temp = pointer_tetris->highscores[max_idx].score;
		pointer_tetris->highscores[max_idx].score = pointer_tetris->highscores[i].score;
		pointer_tetris->highscores[i].score = temp;

		char tempc[10];
		snprintf(tempc,10,"%s",pointer_tetris->highscores[max_idx].name);
		//snprintf(pointer_tetris->highscores[max_idx].name,10,"%s",pointer_tetris->highscores[i].name);
		snprintf(pointer_tetris->highscores[i].name,10,"%s",tempc);
    }

	for(uint8_t i = 0;i < SCORE_LENGTH;i++){
		tetrisgame_saveHighscore(i,pointer_tetris->highscores[i]);
	}
	
}