#ifndef _LALEMA_GAME_TETRIS_H_
#define _LALEMA_GAME_TETRIS_H_

#include <HLM_game.h>

#ifdef __cplusplus
extern "C" {
#endif
	
void* tetrisgame_setup();
void tetrisgame_loop(void*);
void tetrisgame_free(void*);

extern const HLM_game tetris_gamedesc;

#ifdef __cplusplus
}
#endif

#endif

