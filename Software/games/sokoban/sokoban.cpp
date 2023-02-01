
#include "sokoban.h"
#include "level.h"
//#include <esp_log.h>
//#include "../../graphics.h"
#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include <HLM_storage.h>
#include <HLM_time.h>
#include <cstdlib>
#include <cstdio>

#include "levels.c"

typedef struct sokobanmem_t {
	bool ingame;
	uint8_t levelnum;
	uint8_t worldnum;
	uint8_t levelprogress;
	class sokoban_level level;
	int16_t next_hold_auto_input_ms;
	int64_t last_frame_time;
	bool last_left;
	bool last_right;
	bool last_up;
	bool last_down;
	bool last_undo;
	bool last_menu;
	bool last_primary;
} sokobanmem_t;

#define SOKOMEM ((sokobanmem_t*)gamemem)

#define PLAYERHEIGHT 500
#define GAMEHEIGHT 3200
#define GAMEWIDTH 6400

//#define SOKOBAN_STORAGE_KEY "SokobanProgress"
static const char *SOKOBAN_STORAGE_KEYS[WORLDCOUNT] = {"SokobanProgress","SokobanW1","SokobanW2","SokobanW3","SokobanW4","SokobanW5","SokobanW6"};
//static const char *TAG = "Sokoban";
const uint8_t icon[] = {0x00, 0x01, 0x20, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC9, 0x86, 0xEE, 0x68, 0xEE, 0x68, 0x00, 0x00, 0xEE, 0x68, 0x49, 0xB8, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x86, 0x00, 0x00, 0xC9, 0x86, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const HLM_game sokoban_game = {
	"Sokoban",
	icon,
	*sokoban_setup,
	*sokoban_loop,
	*sokoban_free
};

void get_level_progress(void* gamemem){
	SOKOMEM->levelprogress = 0;
	if (HLM_storage_exists32(SOKOBAN_STORAGE_KEYS[SOKOMEM->worldnum])) {
		SOKOMEM->levelprogress = HLM_storage_read32(SOKOBAN_STORAGE_KEYS[SOKOMEM->worldnum]);
		printf("read ret: %d\n", SOKOMEM->levelprogress);
	}
	SOKOMEM->levelnum = SOKOMEM->levelprogress;
	if(SOKOMEM->levelprogress == worlds[SOKOMEM->worldnum].levelnum){
		SOKOMEM->levelnum = SOKOMEM->levelprogress - 1;
	}	
}

void* sokoban_setup() {
	void* gamemem = malloc(sizeof(sokobanmem_t));
	SOKOMEM->ingame = false;
	SOKOMEM->worldnum = 0;
	get_level_progress(gamemem);	
	printf("init\n");
	SOKOMEM->last_undo = isPrimaryButtonPressed(1);
	SOKOMEM->last_left = getLRInput(1) < -60;
	SOKOMEM->last_right = getLRInput(1) > 60;
	SOKOMEM->last_up = getUDInput(1) < -60;
	SOKOMEM->last_down = getUDInput(1) > 60;
	SOKOMEM->last_menu = isMenuButtonPressed(1);
	SOKOMEM->last_primary = isPrimaryButtonPressed(1) || isSecondaryButtonPressed(1);

	return gamemem;
}
void sokoban_loop(void* gamemem) {
	uint16_t delta = get_ms_since_boot() - SOKOMEM->last_frame_time;
	if (SOKOMEM->next_hold_auto_input_ms != 0) {
		SOKOMEM->next_hold_auto_input_ms -= delta;
		if (SOKOMEM->next_hold_auto_input_ms == 0) {
			SOKOMEM->next_hold_auto_input_ms = -1;
		}
	}
	if (getLRInput(1) < -60) {
		if (!SOKOMEM->last_left) {
			if (SOKOMEM->ingame) {
				SOKOMEM->level.go_left();
			} else {
				if (SOKOMEM->levelnum > 0)
					SOKOMEM->levelnum--;
				SOKOMEM->next_hold_auto_input_ms = 500;
			}
		}
		if (!SOKOMEM->ingame && SOKOMEM->next_hold_auto_input_ms < 0) {
			SOKOMEM->next_hold_auto_input_ms = 50;
			if (SOKOMEM->levelnum > 0)
				SOKOMEM->levelnum--;
		}
		SOKOMEM->last_left = true;
	} else {
		if (SOKOMEM->last_left)
			SOKOMEM->next_hold_auto_input_ms = 0;
		SOKOMEM->last_left = false;
	}
	if (getLRInput(1) > 60) {
		if (!SOKOMEM->last_right) {
			if (SOKOMEM->ingame) {
				SOKOMEM->level.go_right();
			} else {
				if (SOKOMEM->levelnum < SOKOMEM->levelprogress && SOKOMEM->levelnum < worlds[SOKOMEM->worldnum].levelnum - 1)
					SOKOMEM->levelnum++;
				SOKOMEM->next_hold_auto_input_ms = 500;
			}
		}
		if (!SOKOMEM->ingame && SOKOMEM->next_hold_auto_input_ms < 0) {
			SOKOMEM->next_hold_auto_input_ms = 50;
			if (SOKOMEM->levelnum < SOKOMEM->levelprogress)
				SOKOMEM->levelnum++;
		}
		SOKOMEM->last_right = true;
	} else {
		if (SOKOMEM->last_right)
			SOKOMEM->next_hold_auto_input_ms = 0;
		SOKOMEM->last_right = false;
	}
	if (getUDInput(1) < -60) {
		if (!SOKOMEM->last_up) {
			if (SOKOMEM->ingame) {
				SOKOMEM->level.go_up();
			} else {
				if (SOKOMEM->worldnum < WORLDCOUNT - 1){
					SOKOMEM->worldnum++;
					get_level_progress(gamemem);
				}
				SOKOMEM->next_hold_auto_input_ms = 500;
			}
		}
		if (!SOKOMEM->ingame && SOKOMEM->next_hold_auto_input_ms < 0) {
			SOKOMEM->next_hold_auto_input_ms = 50;
			if (SOKOMEM->worldnum < WORLDCOUNT - 1){
				SOKOMEM->worldnum++;
				get_level_progress(gamemem);
			}
		}
		SOKOMEM->last_up = true;
	} else {
		if (SOKOMEM->last_up)
			SOKOMEM->next_hold_auto_input_ms = 0;
		SOKOMEM->last_up = false;
	}
	if (getUDInput(1) > 60) {
		if (!SOKOMEM->last_down) {
			if (SOKOMEM->ingame) {
				SOKOMEM->level.go_down();
			} else {
				if (SOKOMEM->worldnum > 0){
					SOKOMEM->worldnum--;
					get_level_progress(gamemem);
				}
				SOKOMEM->next_hold_auto_input_ms = 500;
			}
		}
		if (!SOKOMEM->ingame && SOKOMEM->next_hold_auto_input_ms < 0) {
			SOKOMEM->next_hold_auto_input_ms = 50;
			if (SOKOMEM->worldnum > 0){
				SOKOMEM->worldnum--;
				get_level_progress(gamemem);
			}				
		}
		SOKOMEM->last_down = true;
	} else {
		if (SOKOMEM->last_down)
			SOKOMEM->next_hold_auto_input_ms = 0;
		SOKOMEM->last_down = false;
	}
	if (isPrimaryButtonPressed(1) || isSecondaryButtonPressed(1)) {
		if (!SOKOMEM->last_primary){
			if (!SOKOMEM->ingame) {
				SOKOMEM->level.init(worlds[SOKOMEM->worldnum].levels[SOKOMEM->levelnum]);
				SOKOMEM->ingame = true;
			}
		}
		SOKOMEM->last_primary = true;
	} else {
		SOKOMEM->last_primary = false;
	}
	if (isCoPrimaryButtonPressed(1) || isCoSecondaryButtonPressed(1)) {
		if (!SOKOMEM->last_undo){
			if (SOKOMEM->ingame) {
				SOKOMEM->level.undo();
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
				SOKOMEM->levelprogress++;
				bool ret = HLM_storage_write32(SOKOBAN_STORAGE_KEYS[SOKOMEM->worldnum], (uint32_t)SOKOMEM->levelprogress);
				printf("write ret: %d\n", ret);
			}
			if (SOKOMEM->levelprogress < worlds[SOKOMEM->worldnum].levelnum - 1) {
				SOKOMEM->levelnum++;
			}
			SOKOMEM->ingame = false;
		}
	}
	else
	{
		char leveltext[12];
		uint8_t pixels = snprintf(leveltext, 11, "%s", worlds[SOKOMEM->worldnum].name) * 6;
		get_graphics()->drawText((64 - pixels) / 2, 8, leveltext, 0xFFFF);
		pixels = snprintf(leveltext, 13, "%d/%d", SOKOMEM->levelnum + 1, worlds[SOKOMEM->worldnum].levelnum) * 6;
		get_graphics()->drawText((64 - pixels) / 2, 17, leveltext, 0xFFFF);
		if(SOKOMEM->levelprogress == worlds[SOKOMEM->worldnum].levelnum){
			get_graphics()->drawRect(0,0,64,32,0x07E0);
		}
	}
	SOKOMEM->last_frame_time = get_ms_since_boot();
}
void sokoban_free(void* gamemem) {
	SOKOMEM->level.free();
	free(gamemem);
}
