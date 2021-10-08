#include "tetris_draw.h"

void drawFuncField1p1(uint8_t x, uint8_t y, uint16_t color, MatrixPanel_I2S_DMA* gfx) {
	gfx->drawRect(x*2, y*2, 2, 2, color);
}
void drawFuncHUD1p1(class tetris_game *gameplay, MatrixPanel_I2S_DMA* gfx) {
	char tempstr[10];
	sprintf(tempstr, "%d", gameplay->score);
	drawString(25, 8, tempstr);
	sprintf(tempstr, "%d", gameplay->level);
	drawString(25, 16, tempstr);
	sprintf(tempstr, "%d", gameplay->lines);
	drawString(25, 24, tempstr);
}
