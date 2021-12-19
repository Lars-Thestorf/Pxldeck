#ifndef _LALEMA_GAME_TETRIS_DRAW_H_
#define _LALEMA_GAME_TETRIS_DRAW_H_

#include "stdint.h"
#include <HLM_graphics.h>
#include "tetris_game.h"

void drawFuncField1p1(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx);
void drawFuncHUD1p1(class tetris_game *gameplay, HLM_graphics* gfx);

#endif


