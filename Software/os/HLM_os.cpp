#include <HLM_os.h>
#include <HLM_graphics.h>
#include <HLM_random.h>

#include "mainMenu/mainMenu.h"

#include <cstdio>



void os_init() {
#ifdef FOO
	printf("Foo Jaa\n");
#else
	printf("Foo Nee\n");
#endif
	HLM_graphics *gfx = get_graphics();
	gfx->init();	
}

void os_loop() {
	HLM_graphics *gfx = get_graphics();
	gfx->clear();
	DrawMainMenu();
}
