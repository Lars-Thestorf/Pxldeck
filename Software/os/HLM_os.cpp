#include <HLM_os.h>
#include <HLM_graphics.h>
#include <HLM_random.h>

#include <cstdio>

#include <pong/pong.h>

void* gamemem;

void os_init() {
	//HLM_random(1);
#ifdef FOO
	printf("Jaa\n");
#else
	printf("Nee\n");
#endif
	HLM_graphics *gfx = get_graphics();
	gfx->init();
	gfx->drawPixel(5, 6, 0x2222);
	
	gamemem = pong_game.setupFunction();
}

void os_loop() {
	HLM_graphics *gfx = get_graphics();
	gfx->clear();
	gfx->drawPixel(20, 6, 0x2222);
	pong_game.loopFunction(gamemem);
}
