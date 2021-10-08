#ifndef _LALEMA_GAME_TETRIS_H_
#define _LALEMA_GAME_TETRIS_H_

#include "../gamesApi/gamesApi.h"

#ifdef __cplusplus
extern "C" {
#endif
	
void* tetrisgame_setup();
void tetrisgame_loop(void*);
void tetrisgame_free(void*);

extern const LaLeMaGame tetris_gamedesc;

#ifdef __cplusplus
}
#endif

#endif

