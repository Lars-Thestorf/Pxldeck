#ifndef _LALEMA_GAME_SOKOBAN_H_
#define _LALEMA_GAME_SOKOBAN_H_

#include <HLM_game.h>

#ifdef __cplusplus
extern "C" {
#endif
	
void* sokoban_setup();
void sokoban_loop(void*);
void sokoban_free(void*);

extern const HLM_game sokoban_game;

#ifdef __cplusplus
}
#endif

#endif
