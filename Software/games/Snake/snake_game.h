#ifndef _LALEMA_GAME_SNAKE_H_
#define _LALEMA_GAME_SNAKE_H_

#include <HLM_game.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_FOOD 40
#define NUM_SPEED 3
#define NUM_SLOW 3
#define NUM_IMMORTAL 2
#define NUM_BOMB 3
#define NUM_INVISIBLE 1
#define NUM_EFFECT_POINTS (NUM_FOOD + NUM_SPEED + NUM_SLOW + NUM_IMMORTAL + NUM_BOMB + NUM_INVISIBLE)

#define UPDATE_INTERVALL_RENDER 30
#define BOMB_RADIUS 3.5

void* snakegame_setup();
void snakegame_loop(void*);
void snakegame_free(void*);

extern const HLM_game snake_game;

#ifdef __cplusplus
}
#endif

#endif
