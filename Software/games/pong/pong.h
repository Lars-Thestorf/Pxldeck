#ifndef _LALEMA_GAME_PONG_H_
#define _LALEMA_GAME_PONG_H_

#include <HLM_game.h>

#ifdef __cplusplus
extern "C" {
#endif
	
void* ponggame_setup();
void ponggame_loop(void*);
void ponggame_free(void*);

extern const HLM_game pong_game;

#ifdef __cplusplus
}
#endif

#endif
