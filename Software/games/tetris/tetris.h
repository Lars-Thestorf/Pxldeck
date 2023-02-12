#ifndef _LALEMA_GAME_TETRIS_H_
#define _LALEMA_GAME_TETRIS_H_

#include <HLM_game.h>
#include <HLM_graphics.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCORE_LENGTH 10
typedef struct tetris_highscore_entry_t {
	uint32_t score;
	char name[10];
} tetris_highscore_entry_t;

typedef struct tetrismem_t {
	HLM_graphics* gfx;
	void (* drawFuncField[2])(uint8_t x, uint8_t y, uint16_t color, HLM_graphics* gfx);
	void (* drawFuncHUD[2])(tetrismem_t *mem, HLM_graphics* gfx);
	tetris_highscore_entry_t highscores[SCORE_LENGTH];
    uint8_t highscoreIndex;
	class tetris_game *gameplay[2];
	char name[2][10];
    uint8_t playerCount;
} tetrismem_t;

void* tetrisgame_setup();
void tetrisgame_loop(void*);
void tetrisgame_free(void*);

extern const HLM_game tetris_gamedesc;

#ifdef __cplusplus
}
#endif

#endif

