
#include "sokoban.h"
#include "level.h"
//#include <esp_log.h>
//#include "../../graphics.h"
#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include <HLM_storage.h>
#include <cstdlib>
#include <cstdio>

#include "levels.c"

typedef struct sokobanmem_t {
	bool ingame;
	uint8_t levelnum;
	uint8_t levelprogress;
	class sokoban_level level;
	bool last_left;
	bool last_right;
	bool last_up;
	bool last_down;
	bool last_undo;
	bool last_menu;
} sokobanmem_t;

#define SOKOMEM ((sokobanmem_t*)gamemem)

#define PLAYERHEIGHT 500
#define GAMEHEIGHT 3200
#define GAMEWIDTH 6400

#define SOKOBAN_STORAGE_KEY "SokobanProgress"

static const char *TAG = "Pong";

const HLM_game sokoban_game = {
	"Sokoban",
	*sokoban_setup,
	*sokoban_loop,
	*sokoban_free
};

void* sokoban_setup() {
	void* gamemem = malloc(sizeof(sokobanmem_t));
	SOKOMEM->ingame = false;
	SOKOMEM->levelprogress = 0;
	if (HLM_storage_exists32(SOKOBAN_STORAGE_KEY)) {
		SOKOMEM->levelprogress = HLM_storage_read32(SOKOBAN_STORAGE_KEY);
		printf("read ret: %d\n", SOKOMEM->levelprogress);
	}
	SOKOMEM->levelnum = SOKOMEM->levelprogress;
	printf("init\n");
	SOKOMEM->last_undo = isPrimaryButtonPressed(1);
	SOKOMEM->last_left = getLRInput(1) < -60;
	SOKOMEM->last_right = getLRInput(1) > 60;
	SOKOMEM->last_up = getUDInput(1) < -60;
	SOKOMEM->last_down = getUDInput(1) > 60;
	SOKOMEM->last_menu = isMenuButtonPressed(1);
	//SOKOMEM->level.init(levels[2]);
	return gamemem;
}
void sokoban_loop(void* gamemem) {
	if (getLRInput(1) < -60) {
		if (!SOKOMEM->last_left) {
			if (SOKOMEM->ingame) {
				SOKOMEM->level.go_left();
			} else {
				if (SOKOMEM->levelnum > 0)
					SOKOMEM->levelnum--;
			}
		}
		SOKOMEM->last_left = true;
	} else {
		SOKOMEM->last_left = false;
	}
	if (getLRInput(1) > 60) {
		if (!SOKOMEM->last_right) {
			if (SOKOMEM->ingame) {
			SOKOMEM->level.go_right();
			} else {
				if (SOKOMEM->levelnum < SOKOMEM->levelprogress)
					SOKOMEM->levelnum++;
			}
		}
		SOKOMEM->last_right = true;
	} else {
		SOKOMEM->last_right = false;
	}
	if (getUDInput(1) < -60) {
		if(!SOKOMEM->last_up) {
			if (SOKOMEM->ingame)
				SOKOMEM->level.go_up();
		}
		SOKOMEM->last_up = true;
	} else {
		SOKOMEM->last_up = false;
	}
	if (getUDInput(1) > 60) {
		if (!SOKOMEM->last_down) {
			if (SOKOMEM->ingame)
				SOKOMEM->level.go_down();
		}
		SOKOMEM->last_down = true;
	} else {
		SOKOMEM->last_down = false;
	}
	if (isPrimaryButtonPressed(1)) {
		if (!SOKOMEM->last_undo){
			if (SOKOMEM->ingame) {
				SOKOMEM->level.undo();
			} else {
				SOKOMEM->level.init(levels[SOKOMEM->levelnum]);
				SOKOMEM->ingame = true;
			}
		}
		SOKOMEM->last_undo = true;
	} else {
		SOKOMEM->last_undo = false;
	}
	if (isMenuButtonPressed(1)) {
		if (!SOKOMEM->last_menu){
			if (SOKOMEM->ingame) {
				SOKOMEM->ingame = false;
			}
		}
		SOKOMEM->last_menu = true;
	} else {
		SOKOMEM->last_menu = false;
	}
	if (SOKOMEM->ingame) {
		SOKOMEM->level.draw();
		if (SOKOMEM->level.isWon()) {
			if (SOKOMEM->levelnum == SOKOMEM->levelprogress) {
				if (SOKOMEM->levelprogress < LEVELCOUNT) {
					SOKOMEM->levelprogress++;
					SOKOMEM->levelnum++;
					bool ret = HLM_storage_write32(SOKOBAN_STORAGE_KEY, (uint32_t)SOKOMEM->levelprogress);
					printf("write ret: %d\n", ret);
				}
			}
			SOKOMEM->ingame = false;
		}
	}
	else
	{
		char leveltext[12];
		uint8_t pixels = snprintf(leveltext, 10, "Level: %d", SOKOMEM->levelnum + 1) * 6;
		get_graphics()->drawText((64 - pixels) / 2, 12, leveltext, 0xFFFF);
	}
}
void sokoban_free(void* gamemem) {
	SOKOMEM->level.free();
	free(gamemem);
}
