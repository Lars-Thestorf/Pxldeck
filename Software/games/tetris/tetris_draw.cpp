#include "tetris_draw.h"

#include <cstdio>

void drawFuncField1p1(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx) {
	gfx->drawRect((x+1)*2, y*2, 2, 2, color);
}
void drawFuncHUD1p1(tetrismem_t *mem, HLM_graphics* gfx) {
	char tempstr[12];
	uint8_t pixel;
	switch(mem->gameplay[0]->game_state) {
		case GAME_STATE_MENU:
			snprintf(tempstr, 11, "Level: %d", mem->gameplay[0]->level);
			gfx->drawText(8, 1, tempstr, 0xFFFF);
			snprintf(tempstr, 11, "Highscores");
			gfx->drawText(2, 8, tempstr, 0xFFFF);
			pixel = snprintf(tempstr, 11, "%d. %d", mem->highscoreIndex + 1, mem->highscores[mem->highscoreIndex].score) * 6;
			gfx->drawText((64 - pixel) / 2, 16, tempstr, 0xFFFF);
			pixel = snprintf(tempstr, 11, "%s", mem->highscores[mem->highscoreIndex].name) * 6;
			gfx->drawText((64 - pixel) / 2, 24, tempstr, 0xFFFF);
			break;
		case GAME_STATE_PLAY:
			gfx->drawRect(0, 0, 2, 32, 0x8710);
			gfx->drawRect(22, 0, 2, 32, 0x8710);
			snprintf(tempstr, 11, "%d", mem->gameplay[0]->score);
			gfx->drawText(25, 8, tempstr, 0xFFFF);
			snprintf(tempstr, 11, "%d", mem->gameplay[0]->level);
			gfx->drawText(25, 16, tempstr, 0xFFFF);
			snprintf(tempstr, 11, "%d", mem->gameplay[0]->lines);
			gfx->drawText(25, 24, tempstr, 0xFFFF);
			break;
		case GAME_STATE_OVER:
			snprintf(tempstr, 11, "Game Over");
			gfx->drawText(6, 4, tempstr, 0xFFFF);
			snprintf(tempstr, 11, "Score:");
			gfx->drawText(6, 12, tempstr, 0xFFFF);
			snprintf(tempstr, 11, "%d", mem->gameplay[0]->score);
			gfx->drawText(6, 21, tempstr, 0xFFFF);
			break;
	}
}
