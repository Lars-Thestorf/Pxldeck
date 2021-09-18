
#include "pong.h"
#include <esp_log.h>
#include "../../graphics.h"
#include "../../playerInput/playerInput.h"

typedef struct pongmem_t {
	int8_t p1pos;
	int8_t p2pos;
	int8_t bxpos;
	int8_t bypos;
	int8_t bxspeed;
	int8_t byspeed;
	uint8_t p1score;
	uint8_t p2score;
} pongmem_t;

#define PONGMEM ((pongmem_t*)gamemem)

#define PLAYERHEIGHT 5
#define GAMEHEIGHT 32
#define GAMEWIDTH 64

static const char *TAG = "Pong";

const LaLeMaGame pong_game = {
	"Pong",
	*ponggame_setup,
	*ponggame_loop,
	*ponggame_free
};

void* ponggame_setup() {
	ESP_LOGI(TAG, "PongStart");
	pongmem_t *pointer = (pongmem_t*)malloc(sizeof(pongmem_t));
	pointer->p1pos = GAMEHEIGHT / 2;
	pointer->p2pos = GAMEHEIGHT / 2;
	pointer->bxspeed = 1;
	pointer->byspeed = 1;
	pointer->p1score = 0;
	pointer->p2score = 0;
	pointer->bxpos = 32;
	pointer->bypos = 16;
	return pointer;
}
void ponggame_loop(void* gamemem) {
	//ESP_LOGI(TAG, "PongLoop");
	vTaskDelay(20 / portTICK_PERIOD_MS); // TODO: das ist böse xD
	MatrixPanel_I2S_DMA *gfx = getGraphics();
	
	//score
	uint16_t gray = GRAPHICS_COLOR_RED1 | GRAPHICS_COLOR_RED2 | GRAPHICS_COLOR_RED3 | GRAPHICS_COLOR_GREEN1 | GRAPHICS_COLOR_GREEN2 | GRAPHICS_COLOR_GREEN3 | GRAPHICS_COLOR_BLUE1 | GRAPHICS_COLOR_BLUE2 | GRAPHICS_COLOR_BLUE3;
	gfx->setTextColor(gray);
	char score[4];
	sprintf(score, "%d", PONGMEM->p1score);
	drawString(13,0, score);
	sprintf(score, "%d", PONGMEM->p2score);
	drawString(45,0, score);
	
	//middle
	gfx->drawFastVLine(GAMEWIDTH/2, 0, GAMEHEIGHT, gray);
	gfx->drawFastVLine(GAMEWIDTH/2 - 1, 0, GAMEHEIGHT, gray);
	
	//left player
	int8_t delta = getUDInput(1) / 60;
	if (PONGMEM->p1pos + delta < PLAYERHEIGHT / 2)
		PONGMEM->p1pos = PLAYERHEIGHT / 2;
	else if (PONGMEM->p1pos + delta > GAMEHEIGHT - PLAYERHEIGHT / 2 - 1)
		PONGMEM->p1pos = GAMEHEIGHT - PLAYERHEIGHT / 2 - 1;
	else
		PONGMEM->p1pos += delta;
	gfx->drawFastVLine(0, PONGMEM->p1pos - PLAYERHEIGHT / 2, PLAYERHEIGHT, GRAPHICS_COLOR_WHITE);
	
	//right player
	delta = getRJoystickYAxis(1) / 60;
	if (PONGMEM->p2pos + delta < 2)
		PONGMEM->p2pos = 2;
	else if (PONGMEM->p2pos + delta > 29)
		PONGMEM->p2pos = 29;
	else
		PONGMEM->p2pos += delta;
	gfx->drawFastVLine(GAMEWIDTH - 1, PONGMEM->p2pos - PLAYERHEIGHT / 2, PLAYERHEIGHT, GRAPHICS_COLOR_WHITE);
	
	//ball
	PONGMEM->bxpos += PONGMEM->bxspeed;
	PONGMEM->bypos += PONGMEM->byspeed;
	if (PONGMEM->bypos <= 0)
		PONGMEM->byspeed = -PONGMEM->byspeed;
	if (PONGMEM->bypos >= GAMEHEIGHT - 1)
		PONGMEM->byspeed = -PONGMEM->byspeed;
	if (PONGMEM->bxpos <= 1) {
		if ((PONGMEM->bypos >= PONGMEM->p1pos - PLAYERHEIGHT / 2) && (PONGMEM->bypos <= PONGMEM->p1pos + PLAYERHEIGHT / 2)) {
			PONGMEM->bxspeed = -PONGMEM->bxspeed;
		} else {
			PONGMEM->p2score += 1;
			PONGMEM->bxpos = GAMEWIDTH / 2;
			PONGMEM->bypos = GAMEHEIGHT / 2;
			PONGMEM->bxspeed = 1;
			PONGMEM->byspeed = -1;
		}
	}
	if (PONGMEM->bxpos >= GAMEWIDTH - 2) {
		if ((PONGMEM->bypos >= PONGMEM->p2pos - PLAYERHEIGHT / 2) && (PONGMEM->bypos <= PONGMEM->p2pos + PLAYERHEIGHT / 2)) {
			PONGMEM->bxspeed = -PONGMEM->bxspeed;
		} else {
			PONGMEM->p1score += 1;
			PONGMEM->bxpos = 32;
			PONGMEM->bypos = 16;
			PONGMEM->bxspeed = -1;
			PONGMEM->byspeed = 1;
		}
	}
	gfx->drawPixel(PONGMEM->bxpos, PONGMEM->bypos, GRAPHICS_COLOR_WHITE);
}
void ponggame_free(void* gamemem) {
	free(gamemem);
	ESP_LOGI(TAG, "PongFree");
}
