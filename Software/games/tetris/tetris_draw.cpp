#include "tetris_draw.h"

#include <cstdio>

const unsigned char font_small[96][3] = {
	{0x00,0x00,0x00}, //  
	{0x00,0x2e,0x00}, // !
	{0x06,0x00,0x06}, // "
	{0x3e,0x14,0x3e}, // #
	{0x0c,0x36,0x18}, // $
	{0x32,0x08,0x26}, // %
	{0x14,0x2a,0x34}, // &
	{0x00,0x06,0x00}, // '
	{0x00,0x1c,0x22}, // (
	{0x22,0x1c,0x00}, // )
	{0x14,0x08,0x14}, // *
	{0x08,0x1c,0x08}, // +
	{0x40,0x60,0x00}, // ,
	{0x08,0x08,0x08}, // -
	{0x00,0x20,0x00}, // .
	{0x60,0x1c,0x02}, // /
	{0x3e,0x22,0x3e}, // 0
	{0x24,0x3e,0x20}, // 1
	{0x32,0x2a,0x26}, // 2
	{0x22,0x2a,0x3e}, // 3
	{0x0e,0x08,0x3e}, // 4
	{0x2e,0x2a,0x3a}, // 5
	{0x3e,0x2a,0x3a}, // 6
	{0x02,0x3a,0x06}, // 7
	{0x3e,0x2a,0x3e}, // 8
	{0x2e,0x2a,0x3e}, // 9
	{0x00,0x14,0x00}, // :
	{0x40,0x64,0x00}, // ;
	{0x08,0x14,0x22}, // <
	{0x14,0x14,0x14}, // =
	{0x22,0x14,0x08}, // >
	{0x02,0x2a,0x06}, // ?
	{0x3e,0x22,0x2e}, // @
	{0x3e,0x0a,0x3e}, // A
	{0x3e,0x2a,0x36}, // B
	{0x3e,0x22,0x22}, // C
	{0x3e,0x22,0x1c}, // D
	{0x3e,0x2a,0x22}, // E
	{0x3e,0x0a,0x02}, // F
	{0x3e,0x22,0x3a}, // G
	{0x3e,0x08,0x3e}, // H
	{0x22,0x3e,0x22}, // I
	{0x30,0x20,0x3e}, // J
	{0x3e,0x08,0x36}, // K
	{0x3e,0x20,0x20}, // L
	{0x3e,0x04,0x3e}, // M
	{0x3c,0x08,0x1e}, // N
	{0x3e,0x22,0x3e}, // O
	{0x3e,0x0a,0x0e}, // P
	{0x3e,0x22,0x7e}, // Q
	{0x3e,0x0a,0x34}, // R
	{0x2e,0x2a,0x3a}, // S
	{0x02,0x3e,0x02}, // T
	{0x3e,0x20,0x3e}, // U
	{0x1e,0x20,0x1e}, // V
	{0x3e,0x10,0x3e}, // W
	{0x36,0x08,0x36}, // X
	{0x06,0x38,0x06}, // Y
	{0x32,0x2a,0x26}, // Z
	{0x00,0x3e,0x22}, // [
	{0x02,0x1c,0x60}, // "\"
	{0x22,0x3e,0x00}, // ]
	{0x04,0x02,0x04}, // ^
	{0x40,0x40,0x40}, // _
	{0x00,0x02,0x00}, // `
	{0x3e,0x0a,0x3e}, // a
	{0x3e,0x2a,0x36}, // b
	{0x3e,0x22,0x22}, // c
	{0x3e,0x22,0x1c}, // d
	{0x3e,0x2a,0x22}, // e
	{0x3e,0x0a,0x02}, // f
	{0x3e,0x22,0x3a}, // g
	{0x3e,0x08,0x3e}, // h
	{0x22,0x3e,0x22}, // i
	{0x30,0x20,0x3e}, // j
	{0x3e,0x08,0x36}, // k
	{0x3e,0x20,0x20}, // l
	{0x3e,0x04,0x3e}, // m
	{0x3c,0x08,0x1e}, // n
	{0x3e,0x22,0x3e}, // o
	{0x3e,0x0a,0x0e}, // p
	{0x3e,0x22,0x7e}, // q
	{0x3e,0x0a,0x34}, // r
	{0x2e,0x2a,0x3a}, // s
	{0x02,0x3e,0x02}, // t
	{0x3e,0x20,0x3e}, // u
	{0x1e,0x20,0x1e}, // v
	{0x3e,0x10,0x3e}, // w
	{0x36,0x08,0x36}, // x
	{0x06,0x38,0x06}, // y
	{0x32,0x2a,0x26}, // z
	{0x08,0x3e,0x22}, // {
	{0x00,0x7e,0x00}, // |
	{0x22,0x3e,0x08}, // }
	{0x02,0x06,0x04}, // ~
	{0x00,0x00,0x00}
};

void drawSmallText(uint8_t x, uint8_t y, char* str, uint16_t color, HLM_graphics* gfx, Direction dir){
	while (*str) {
        uint8_t i,j;
		// Convert the character to an index
		char c = *str & 0x7F;
		if (c < ' ') {
			c = 0;
		} else {
			c -= ' ';
		}
		const uint8_t* chr = font_small[(uint8_t)c];

		switch (dir)
		{
		case UP:
		// Draw pixels
			for (j=0; j<3; j++) {
				for (i=1; i<6; i++) {
					if (chr[j] & (1<<i)) {
						gfx->drawPixel(x +  i - 1, y + 2 - j, color);
					}
				}
			}
			y -= 4;
			break;
		case DOWN:
		// Draw pixels
			for (j=0; j<3; j++) {
				for (i=1; i<6; i++) {
					if (chr[j] & (1<<i)) {
						gfx->drawPixel(x +  4 - (i - 1), y + j, color);
					}
				}
			}
			y += 4;
			break;
		case RIGHT:
			// Draw pixels
			for (j=0; j<3; j++) {
				for (i=1; i<6; i++) {
					if (chr[j] & (1<<i)) {
						gfx->drawPixel(x + j, y + i - 1, color);
					}
				}
			}
			x += 4;
			break;
		case LEFT:
		// Draw pixels
			for (j=0; j<3; j++) {
				for (i=1; i<6; i++) {
					if (chr[j] & (1<<i)) {
						gfx->drawPixel(x + 2 - j, y + 4 - (i - 1), color);
					}
				}
			}
			x -= 4;
			break;
		
		default:
			break;
		}
		str++;
    }
}
void orientation_32_32_section(uint8_t *x, uint8_t *y, Direction dir){
	bool leftSide = true;
	if(*x >= 32){
		*x -= 32;
		leftSide = false;
	}
	uint8_t temp = *x;
	switch (dir)
	{
	case UP:

		break;
	case DOWN:
		*x = 31 - *x;
		*y = 31 - *y;
		break;
	case LEFT:
		*x = *y;
		*y = 31 - temp;
		break;
	case RIGHT:
		*x = 31 - *y;
		*y = temp;
		break;
	
	default:
		break;
	}
	if(!leftSide){
		*x += 32;
	}
}
void drawFuncField1p1(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx) {
	gfx->drawRect((x+1)*2, y*2, 2, 2, color);
}
void drawFuncField2p1(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx) {
	x *= 2;
	y *= 2;
	orientation_32_32_section(&x,&y,RIGHT);
	gfx->drawRect(x, y, 2, 2, color);
}
void drawFuncField2p2(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx) {
	x *= 2;
	x += 32;
	y *= 2;
	orientation_32_32_section(&x,&y,LEFT);
	gfx->drawRect(x, y, 2, 2, color);
}

void drawFuncHUD2p1(tetrismem_t *mem, HLM_graphics* gfx) {
	char tempstr[12];
	uint8_t pixel;
	switch(mem->gameplay[0]->game_state) {
		case GAME_STATE_MENU:
			snprintf(tempstr, 11, "Level: %d", mem->gameplay[0]->level);
			drawSmallText(25, 0, tempstr, 0xffff, gfx, DOWN);
			snprintf(tempstr, 11, "Highscore");
			drawSmallText(19, 0, tempstr, 0xffff, gfx, DOWN);
			pixel = snprintf(tempstr, 11, "%d. %ld", mem->highscoreIndex + 1, mem->highscores[mem->highscoreIndex].score) * 6;
			drawSmallText(13, 0, tempstr, 0xffff, gfx, DOWN);
			pixel = snprintf(tempstr, 11, "%s", mem->highscores[mem->highscoreIndex].name) * 6;
			drawSmallText(7, 0, tempstr, 0xffff, gfx, DOWN);


			break;
		case GAME_STATE_PLAY:
			gfx->fillRect(0, FIELD_WIDTH * 2, 32, 12, 0x10);

			snprintf(tempstr, 11, "%ld", mem->gameplay[0]->score);
			drawSmallText(20, 21, tempstr, 0xffff, gfx, DOWN);
			snprintf(tempstr, 11, "%d", mem->gameplay[0]->level);
			drawSmallText(14, 21, tempstr, 0xffff, gfx, DOWN);
			snprintf(tempstr, 11, "%d", mem->gameplay[0]->lines);
			drawSmallText(8, 21, tempstr, 0xffff, gfx, DOWN);
			break;
		case GAME_STATE_OVER:
			snprintf(tempstr, 11, "GameOver");
			drawSmallText(25, 0, tempstr, 0xffff, gfx, DOWN);
			snprintf(tempstr, 11, "Score:");
			drawSmallText(19, 0, tempstr, 0xffff, gfx, DOWN);
			snprintf(tempstr, 11, "%ld", mem->gameplay[0]->score);
			drawSmallText(13, 0, tempstr, 0xffff, gfx, DOWN);
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
			pixel = snprintf(tempstr, 11, "%d. %ld", mem->highscoreIndex + 1, mem->highscores[mem->highscoreIndex].score) * 6;
			gfx->drawText((64 - pixel) / 2, 16, tempstr, 0xFFFF);
			pixel = snprintf(tempstr, 11, "%s", mem->highscores[mem->highscoreIndex].name) * 6;
			gfx->drawText((64 - pixel) / 2, 24, tempstr, 0xFFFF);
			break;
		case GAME_STATE_PLAY:
			gfx->drawRect(0, 0, 2, 32, 0x8710);
			gfx->drawRect(22, 0, 2, 32, 0x8710);
			snprintf(tempstr, 11, "%ld", mem->gameplay[0]->score);
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
			snprintf(tempstr, 11, "%ld", mem->gameplay[0]->score);
			gfx->drawText(6, 21, tempstr, 0xFFFF);
			break;
	}
}
