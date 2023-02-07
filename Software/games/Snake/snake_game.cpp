#include "snake_game.h"
#include "snake.h"
#include "effects.h"
#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include <cstdlib>
#include <cstdio>

#include <HLM_random.h>
#include <HLM_time.h>


typedef struct snakemem_t {
  HLM_graphics* gfx;
  Snake snake[8];
  uint8_t num_player;
  uint8_t snakes_alive;
  uint64_t current_time;
  uint64_t prev_time = 0;
  Effect effects[NUM_EFFECT_POINTS];
} snakemem_t;

#define SNAKEGMEM ((snakemem_t*)gamemem)
const uint8_t icon[] = {0x00, 0x01, 0x20, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0xB8, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x00, 0x00, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0xB8, 0x00, 0x00, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x86, 0x00, 0x00, 0x00, 0x00, 0xF7, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x86, 0xC9, 0x86, 0xC9, 0x86, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x65, 0x8A, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x86, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const HLM_game snake_game = {
	"Snake",
  icon,
	*snakegame_setup,
	*snakegame_loop,
	*snakegame_free
};

bool isOnEffect(Point p,snakemem_t* pointer) {
  for (int i = 0; i < NUM_EFFECT_POINTS; i++) {
    if (pointer->effects[i].isOnPoint(p)) {
      return true;
    }
  }
  return false;
}

bool isOnAnySnake(Point p,snakemem_t* pointer) {
  bool res = false;
  for (int i = 0; i < pointer->num_player; i++) {
        res |= pointer->snake[i].isOnSnakeOrHead(p);
      }
      return res;
}

Point getEmptyField(snakemem_t* pointer) {
  Point p;
  do {
      p.x = HLM_random(64);
      p.y = HLM_random(32);
  } while (isOnAnySnake(p,pointer) || isOnEffect(p,pointer));
  return p;
}

void renderEffects(snakemem_t* pointer) {
  for (int i = 0; i < NUM_EFFECT_POINTS; i++) {
    pointer->effects[i].render();
  }
}

void setupEffects(snakemem_t* pointer) {
  int i = 0;
  for (; i < NUM_FOOD; i++)
      pointer->effects[i] = Food();
  for (int a = 0; a < NUM_SPEED; a++, i++)
      pointer->effects[i] = Speed();
  for (int a = 0; a < NUM_SLOW; a++, i++)
      pointer->effects[i] = Slow();
  for (int a = 0; a < NUM_IMMORTAL; a++, i++)
      pointer->effects[i] = Immortal();
  for (int a = 0; a < NUM_BOMB; a++, i++)
      pointer->effects[i] = Bomb();
  for (int a = 0; a < NUM_INVISIBLE; a++, i++)
      pointer->effects[i] = Invisible();
  for (int i = 0; i < NUM_EFFECT_POINTS; i++)
      pointer->effects[i].spawn(getEmptyField(pointer));
  renderEffects(pointer);
}

void checkCollision(snakemem_t* pointer) {
  for (int i = 0; i < pointer->num_player; i++) {
    for (int j = 0; j < pointer->num_player; j++) {
      if (pointer->snake[j].isImmortal()) {
        continue;
      }
      if (i == j) {
        if (pointer->snake[i].isOnSnake(pointer->snake[i].head->p)) {
          pointer->snake[i].removeSnake();
        }
      } else {
        if (pointer->snake[i].isOnSnakeOrHead(pointer->snake[j].head->p)) {
          pointer->snake[j].removeSnake();
        }
      }     
    }
  }
}
    
void checkEffects(snakemem_t* pointer) {
  for (int e = 0; e < NUM_EFFECT_POINTS; e++) {
    for (int s = 0; s < pointer->num_player; s++) {
      if (pointer->effects[e].isOnPoint(pointer->snake[s].head->p)) {
        pointer->effects[e].spawn(getEmptyField(pointer));
        switch (pointer->effects[e].type) {
          case FOOD: pointer->snake[s].increaseSize(); break;
          case SPEED: pointer->snake[s].increaseSpeed(); break;
          case SLOW: pointer->snake[s].decreaseSpeed(); break;
          case IMMORTAL: pointer->snake[s].makeImmortal(); break;
          case BOMB: break;
          case INVISIBLE: pointer->snake[s].makeInvisible();break;
          case NONE: break;
        }
      }
    }
  }
}

void handleDirectionChanges(snakemem_t* pointer) {
  for(uint8_t i = 0;i <pointer->num_player;i++){
    if(getLRInput(i + 1) < -60){
      pointer->snake[i].setDirection(LEFT);
    }else if(getLRInput(i + 1) > 60){
      pointer->snake[i].setDirection(RIGHT);
    }else if(getUDInput(i + 1) < -60){
      pointer->snake[i].setDirection(DOWN);
    }else if(getUDInput(i + 1) > 60){
      pointer->snake[i].setDirection(UP);
    }
  }
}

void updateSnakes(void* gamemem) {
      SNAKEGMEM->current_time = get_ms_since_boot();
      for(uint8_t i = 0;i < SNAKEGMEM->num_player;i++){
        SNAKEGMEM->snake[i].render();
        SNAKEGMEM->snake[i].updateSnake(SNAKEGMEM->current_time);
      }
      renderEffects((snakemem_t*)gamemem);
      checkEffects((snakemem_t*)gamemem);
      checkCollision((snakemem_t*)gamemem);
      handleDirectionChanges((snakemem_t*)gamemem);
}

void* snakegame_setup() {
	snakemem_t* pointer = (snakemem_t*)malloc(sizeof(snakemem_t));
  pointer->num_player = 2;
	pointer->gfx = get_graphics();
  for (uint8_t i = 0; i < pointer->num_player; i++){
    pointer->snake[i] = Snake(i, newPoint(i*8+3,10));
	  pointer->snake[i].render();
  }
	setupEffects(pointer);
	return pointer;
}

void snakegame_loop(void* gamemem) {
	updateSnakes(gamemem);
}

void snakegame_free(void* gamemem) {
	free(gamemem);
}
