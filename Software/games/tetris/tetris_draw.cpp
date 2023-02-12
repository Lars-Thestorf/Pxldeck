#include "tetris_draw.h"

#include <cstdio>
uint8_t number[10][3] = {
	{0x75,0x55,0x70},
	{0x11,0x11,0x10},
	{0x71,0x74,0x70},
	{0x71,0x71,0x70},
	{0x55,0x71,0x10},
	{0x74,0x71,0x70},
	{0x44,0x75,0x70},
	{0x71,0x22,0x40},
	{0x75,0x75,0x70},
	{0x75,0x71,0x10}
};

void drawNumber(uint8_t x, uint8_t y, char* str, uint16_t color, HLM_graphics* gfx){
	int pos = 0;
	char c = str[pos];
	while(c != '\0') {
		uint16_t addr = c - 48;
		for (uint8_t col = 0; col < 3; col++) {
			if (number[addr][col] & 0x04) gfx->drawPixel(x + 0, y + col * 2 + 1, color);
			if (number[addr][col] & 0x02) gfx->drawPixel(x + 1, y + col * 2 + 1, color);
			if (number[addr][col] & 0x01) gfx->drawPixel(x + 2, y + col * 2 + 1, color);
			if (number[addr][col] & 0x40) gfx->drawPixel(x + 0, y + col * 2, color);
			if (number[addr][col] & 0x20) gfx->drawPixel(x + 1, y + col * 2, color);
			if (number[addr][col] & 0x10) gfx->drawPixel(x + 2, y + col * 2, color);
		}
		x+=4;
		pos++;
		c = str[pos];
	}
}
void drawFuncField1p1(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx) {
	gfx->drawRect((x+1)*2, y*2, 2, 2, color);
}
void drawFuncField2p1(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx) {
	gfx->drawRect((FIELD_HEIGHT - 1 -y)*2, x*2, 2, 2, color);
}
void drawFuncField2p2(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx) {
	gfx->drawRect((y + FIELD_HEIGHT) * 2, (FIELD_WIDTH - 1 -x)*2 + 12, 2, 2, color);
}

void drawFuncHUD2p1(tetrismem_t *mem, HLM_graphics* gfx) {
	char tempstr[12];
	uint8_t pixel;
	switch(mem->gameplay[0]->game_state) {
		case GAME_STATE_MENU:
			drawNumber(0,0,"1234",0xffff,gfx);
			break;
		case GAME_STATE_PLAY:
			gfx->drawRect(0, FIELD_WIDTH * 2, 32, 12, 0x8710);
			break;
		case GAME_STATE_OVER:

			break;
	}
}

void drawFuncHUD2p2(tetrismem_t *mem, HLM_graphics* gfx) {
	char tempstr[12];
	uint8_t pixel;
	switch(mem->gameplay[1]->game_state) {
		case GAME_STATE_MENU:

			break;
		case GAME_STATE_PLAY:
			gfx->drawRect(FIELD_HEIGHT * 2, 0, 32, 12, 0x8710);
			break;
		case GAME_STATE_OVER:

			break;
	}
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
