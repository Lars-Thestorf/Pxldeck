#include <HLM_os.h>
#include <HLM_graphics.h>
#include <HLM_random.h>
#include <HLM_storage.h>

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
	printf("init");
	if (HLM_storage_exists32("Brightness")) {
		printf("set Brightness");
		gfx->setBrightness(HLM_storage_read32("Brightness"));
	}else{
		gfx->setBrightness(10);
	}	
}

void os_loop() {
	HLM_graphics *gfx = get_graphics();
	gfx->clear();
	DrawMainMenu();
}
