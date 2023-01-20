#include "snake_game.h"
#include "snake.h"
#include "effects.h"
//#include <esp_log.h>
//#include "../../graphics.h"
#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include <cstdlib>
#include <cstdio>

#include <HLM_random.h>
#include <HLM_time.h>


typedef struct snakemem_t {
  HLM_graphics* gfx;
  Snake snake;
  uint64_t current_time;
  uint64_t prev_time = 0;
  Effect effects[NUM_EFFECT_POINTS];
} pongmem_t;

#define SNAKEGMEM ((pongmem_t*)gamemem)
static const char *TAG = "Snake";

const HLM_game snake_game = {
	"Snake",
	*snakegame_setup,
	*snakegame_loop,
	*snakegame_free
};

bool isOnEffect(Point p,pongmem_t* pointer) {
  for (int i = 0; i < NUM_EFFECT_POINTS; i++) {
    if (pointer->effects[i].isOnPoint(p)) {
      return true;
    }
  }
  return false;
}

bool isOnAnySnake(Point p,pongmem_t* pointer) {
  bool res = false;
  for (int i = 0; i < 4; i++) {
      pointer->snake.isOnSnakeOrHead(p);
  }
  return res;
}

Point getEmptyField(pongmem_t* pointer) {
  Point p;
  do {
      p.x = HLM_random(64);
      p.y = HLM_random(32);
  } while (isOnAnySnake(p,pointer) || isOnEffect(p,pointer));
  return p;
}

void renderEffects(pongmem_t* pointer) {
  for (int i = 0; i < NUM_EFFECT_POINTS; i++) {
    pointer->effects[i].render();
  }
}

void setupEffects(pongmem_t* pointer) {
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

void checkCollision(pongmem_t* pointer) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (pointer->snake.isImmortal()) {
          continue;
      }
      if (pointer->snake.isOnSnake(pointer->snake.head->p)) {
          pointer->snake.removeSnake();
      }      
    }
  }
}
    
void checkEffects(pongmem_t* pointer) {
  for (int e = 0; e < NUM_EFFECT_POINTS; e++) {
    for (int s = 0; s < 4; s++) {
      if (pointer->effects[e].isOnPoint(pointer->snake.head->p)) {
        pointer->effects[e].spawn(getEmptyField(pointer));
        switch (pointer->effects[e].type) {
          case FOOD: pointer->snake.increaseSize(); break;
          case SPEED: pointer->snake.increaseSpeed(); break;
          case SLOW: pointer->snake.decreaseSpeed(); break;
          case IMMORTAL: pointer->snake.makeImmortal(); break;
          case BOMB: break;
          case INVISIBLE: pointer->snake.makeInvisible();break;
          case NONE: break;
        }
      }
    }
  }
}

void handleDirectionChanges(pongmem_t* pointer) {
  if(getLRInput(1) < -60){
    pointer->snake.setDirection(LEFT);
  }else if(getLRInput(1) > 60){
    pointer->snake.setDirection(RIGHT);
  }else if(getUDInput(1) < -60){
    pointer->snake.setDirection(DOWN);
  }else if(getUDInput(1) > 60){
    pointer->snake.setDirection(UP);
  }
}

void updateSnakes(void* gamemem) {
      SNAKEGMEM->current_time = get_ms_since_boot();
      SNAKEGMEM->snake.render();
      renderEffects((pongmem_t*)gamemem);
      SNAKEGMEM->snake.updateSnake(SNAKEGMEM->current_time);
      checkEffects((pongmem_t*)gamemem);
      checkCollision((pongmem_t*)gamemem);
      handleDirectionChanges((pongmem_t*)gamemem);
}

void* snakegame_setup() {
	//ESP_LOGI(TAG, "SnakeStart");
	pongmem_t* pointer = (pongmem_t*)malloc(sizeof(pongmem_t));
	pointer->gfx = get_graphics();
	pointer->snake = Snake(0, newPoint(30,10));
	pointer->snake.render();
	setupEffects(pointer);
	return pointer;
}

void snakegame_loop(void* gamemem) {
	//ESP_LOGI(TAG, "SnakeLoop");
	HLM_graphics *gfx = get_graphics();
	updateSnakes(gamemem);
}

void snakegame_free(void* gamemem) {
	free(gamemem);
	//ESP_LOGI(TAG, "SnakeFree");
}
