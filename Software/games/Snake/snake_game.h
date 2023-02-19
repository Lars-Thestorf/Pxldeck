#ifndef _LALEMA_GAME_SNAKE_H_
#define _LALEMA_GAME_SNAKE_H_

#include <HLM_game.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_FOOD 60
#define NUM_SPEED 6
#define NUM_SLOW 6
#define NUM_IMMORTAL 4
#define NUM_BOMB 6
#define NUM_INVISIBLE 3
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
