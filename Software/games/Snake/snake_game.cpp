
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

#define GRAPHICS_COLOR_WHITE 0xFFFF

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
        p.x = HLM_GRAPHICS_H_(64);
        p.y = HLM_GRAPHICS_H_(32);
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

void* snakegame_setup() {
	//ESP_LOGI(TAG, "SnakeStart");
	pongmem_t* pointer = (pongmem_t*)malloc(sizeof(pongmem_t));
	pointer->gfx = get_graphics();
	pointer->snake = Snake(0, newPoint(30,10));
	pointer->snake.render();
	setupEffects(pointer);


	return pointer;
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
              case BOMB:  break;
              case INVISIBLE: pointer->snake.makeInvisible();break;
              case NONE: break;
            }
          }
        }
      }
    }
void updateSnakes(void* gamemem) {
      SNAKEGMEM->current_time = get_ms_since_boot();
      if (SNAKEGMEM->current_time - SNAKEGMEM->prev_time >= UPDATE_INTERVALL_RENDER) {
        SNAKEGMEM->prev_time = SNAKEGMEM->current_time;
        SNAKEGMEM->snake.render();
        renderEffects((pongmem_t*)gamemem);
      }
      bool mustUpdate = false;
      for (int i = 0; i < 4; i++) {
        mustUpdate |= SNAKEGMEM->snake.updateSnake(SNAKEGMEM->current_time);
      }
      if (mustUpdate) {
        checkEffects((pongmem_t*)gamemem);
        checkCollision((pongmem_t*)gamemem);
      }
    }

void snakegame_loop(void* gamemem) {
	//ESP_LOGI(TAG, "SnakeLoop");
	
	//MatrixPanel_I2S_DMA *gfx = getGraphics();
	HLM_graphics *gfx = get_graphics();
	updateSnakes(gamemem);

	gfx->drawPixel(10, 10, GRAPHICS_COLOR_WHITE);
}
void snakegame_free(void* gamemem) {
	free(gamemem);
	//ESP_LOGI(TAG, "SnakeFree");
}
