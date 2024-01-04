#ifndef _LALEMA_GAME_TICTACTOE_H_
#define _LALEMA_GAME_TICTACTOE_H_

#include <HLM_game.h>

#ifdef __cplusplus
extern "C" {
#endif
	
void* tictactoe_setup();
void tictactoe_loop(void*);
void tictactoe_free(void*);

extern const HLM_game tictactoe_game;

#ifdef __cplusplus
}
#endif

#endif
