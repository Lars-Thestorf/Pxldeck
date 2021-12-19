#include "tetris_draw.h"

#include <cstdio>

void drawFuncField1p1(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx) {
	gfx->drawRect(x*2, y*2, 2, 2, color);
}
void drawFuncHUD1p1(class tetris_game *gameplay, HLM_graphics* gfx) {
	switch(gameplay->game_state) {
		char tempstr[12];
		case GAME_STATE_MENU:
			sprintf(tempstr, "Level: %d", gameplay->level);
			//drawString(8, 12, tempstr);
			break;
		case GAME_STATE_PLAY:
			sprintf(tempstr, "%d", gameplay->score);
			//drawString(25, 8, tempstr);
			sprintf(tempstr, "%d", gameplay->level);
			//drawString(25, 16, tempstr);
			sprintf(tempstr, "%d", gameplay->lines);
			//drawString(25, 24, tempstr);
			break;
		case GAME_STATE_OVER:
			//drawString(6, 4, "Game Over");
			//drawString(6, 12, "Score:");
			sprintf(tempstr, "%d", gameplay->score);
			//drawString(6, 21, tempstr);
			break;
	}
}
