
#include "pong.h"
//#include <esp_log.h>
//#include "../../graphics.h"
#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include <cstdlib>
#include <cstdio>

#include <HLM_random.h>

#define GRAPHICS_COLOR_WHITE 0xFFFF

typedef struct pongmem_t {
	int16_t p1pos;
	int16_t p2pos;
	int16_t bxpos;
	int16_t bypos;
	int16_t bxspeed;
	int16_t byspeed;
	uint8_t p1score;
	uint8_t p2score;
} pongmem_t;

#define PONGMEM ((pongmem_t*)gamemem)

#define PLAYERHEIGHT 500
#define GAMEHEIGHT 3200
#define GAMEWIDTH 6400

static const char *TAG = "Pong";

const HLM_game pong_game = {
	"Pong",
	*ponggame_setup,
	*ponggame_loop,
	*ponggame_free
};

void* ponggame_setup() {
	//ESP_LOGI(TAG, "PongStart");
	pongmem_t *pointer = (pongmem_t*)malloc(sizeof(pongmem_t));
	pointer->p1pos = GAMEHEIGHT / 2;
	pointer->p2pos = GAMEHEIGHT / 2;
	pointer->bxspeed = 30;
	pointer->byspeed = 10;
	pointer->p1score = 0;
	pointer->p2score = 0;
	pointer->bxpos = GAMEWIDTH / 2;
	pointer->bypos = GAMEHEIGHT / 2;
	
	HLM_random(4);
	return pointer;
}
int8_t pongGameInput(uint8_t playernum) {
	switch(getControllerType(playernum)){
		case CONTROLLERTYPE_LEFT:
			return getLRInput(playernum);
		case CONTROLLERTYPE_RIGHT:
			return -getLRInput(playernum);
		default:
			return getUDInput(playernum);
	}
	return 0;
}
void ponggame_loop(void* gamemem) {
	//ESP_LOGI(TAG, "PongLoop");
	
	//MatrixPanel_I2S_DMA *gfx = getGraphics();
	HLM_graphics *gfx = get_graphics();
	
	uint16_t white = 0xFFFF;
	
	//score
	uint16_t gray = 0x3333; //GRAPHICS_COLOR_RED1 | GRAPHICS_COLOR_RED2 | GRAPHICS_COLOR_RED3 | GRAPHICS_COLOR_GREEN1 | GRAPHICS_COLOR_GREEN2 | GRAPHICS_COLOR_GREEN3 | GRAPHICS_COLOR_BLUE1 | GRAPHICS_COLOR_BLUE2 | GRAPHICS_COLOR_BLUE3;
	char score[4];
	sprintf(score, "%d", PONGMEM->p1score);
	gfx->drawText(13,0, score, gray);
	sprintf(score, "%d", PONGMEM->p2score);
	gfx->drawText(45,0, score, gray);
	
	//middle
	gfx->drawVLine(GAMEWIDTH/100/2, 0, GAMEHEIGHT/100, gray);
	gfx->drawVLine(GAMEWIDTH/100/2 - 1, 0, GAMEHEIGHT/100, gray);
	
	//left player
	int8_t delta = pongGameInput(1) / 3;
	if (PONGMEM->p1pos + delta < PLAYERHEIGHT / 2)
		PONGMEM->p1pos = PLAYERHEIGHT / 2;
	else if (PONGMEM->p1pos + delta > GAMEHEIGHT - PLAYERHEIGHT / 2)
		PONGMEM->p1pos = GAMEHEIGHT - PLAYERHEIGHT / 2;
	else
		PONGMEM->p1pos += delta;
	gfx->drawVLine(0, (PONGMEM->p1pos - PLAYERHEIGHT / 2) / 100, PLAYERHEIGHT / 100, GRAPHICS_COLOR_WHITE);
	
	//right player
	if (getPlayerCount() > 1) {
		delta = pongGameInput(2) / 3;
		if (PONGMEM->p2pos + delta < PLAYERHEIGHT / 2)
			PONGMEM->p2pos = PLAYERHEIGHT / 2;
		else if (PONGMEM->p2pos + delta > GAMEHEIGHT - PLAYERHEIGHT / 2)
			PONGMEM->p2pos = GAMEHEIGHT - PLAYERHEIGHT / 2;
		else
			PONGMEM->p2pos += delta;
	} else {
		//CPU enemy
		if (PONGMEM->bypos > PONGMEM->p2pos)
			PONGMEM->p2pos += 20;
		if (PONGMEM->bypos < PONGMEM->p2pos)
			PONGMEM->p2pos -= 20;
	}
	gfx->drawVLine(GAMEWIDTH / 100 - 1, (PONGMEM->p2pos - PLAYERHEIGHT / 2) / 100, PLAYERHEIGHT / 100, white);
	
	//ball
	PONGMEM->bxpos += PONGMEM->bxspeed;
	PONGMEM->bypos += PONGMEM->byspeed;
	if (PONGMEM->bypos <= 0)
		PONGMEM->byspeed = -PONGMEM->byspeed;
	if (PONGMEM->bypos >= GAMEHEIGHT - 1)
		PONGMEM->byspeed = -PONGMEM->byspeed;
	if (PONGMEM->bxpos <= 100) {
		if ((PONGMEM->bypos >= PONGMEM->p1pos - PLAYERHEIGHT / 2) && (PONGMEM->bypos <= PONGMEM->p1pos + PLAYERHEIGHT / 2)) {
			//PONGMEM->bxspeed = -PONGMEM->bxspeed;
			PONGMEM->bxspeed = 30;
			PONGMEM->byspeed = (PONGMEM->bypos - PONGMEM->p1pos) / 5;
		} else {
			PONGMEM->p2score += 1;
			PONGMEM->bxpos = GAMEWIDTH / 2;
			PONGMEM->bypos = GAMEHEIGHT / 2;
			PONGMEM->bxspeed = 30;
			PONGMEM->byspeed = -10;
		}
	}
	if (PONGMEM->bxpos >= GAMEWIDTH - 100) {
		if ((PONGMEM->bypos >= PONGMEM->p2pos - PLAYERHEIGHT / 2) && (PONGMEM->bypos <= PONGMEM->p2pos + PLAYERHEIGHT / 2)) {
			//PONGMEM->bxspeed = -PONGMEM->bxspeed;
			PONGMEM->bxspeed = -30;
			PONGMEM->byspeed = (PONGMEM->bypos - PONGMEM->p2pos) / 5;
		} else {
			PONGMEM->p1score += 1;
			PONGMEM->bxpos = GAMEWIDTH / 2;
			PONGMEM->bypos = GAMEHEIGHT / 2;
			PONGMEM->bxspeed = -30;
			PONGMEM->byspeed = 10;
		}
	}
	gfx->drawPixel(PONGMEM->bxpos/100, PONGMEM->bypos/100, GRAPHICS_COLOR_WHITE);
}
void ponggame_free(void* gamemem) {
	free(gamemem);
	//ESP_LOGI(TAG, "PongFree");
}
