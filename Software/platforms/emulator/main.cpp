#include <SDL2/SDL.h>
#include <stdio.h>
#include "graphics.h"
#include <HLM_graphics.h>
#include "../HMatrix/esp32/main/bootlogo.c"
#include <HLM_os.h>
#include <HLM_random.h>

int main()
{
    graphics_init();
    graphics_drawpixel(1,1,0xFFFF);
	//graphics_drawImage1Bit(0, 0, bootlogo, 64, 32, 0x0000, 0xFFFF);
	
	printf("hey ");
	printf("Die heutige Zahl des Tages: %d\n", HLM_random(42));
	
	os_init();
    
	SDL_Event event;
	// Events mangement
	while(true)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT: // handling of close button
					graphics_uninit();
					return 0;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_SPACE: // Spacebar = Rotate Display
							//set_rotated((horizontal == UCX_DISPLAY_ORIENTATION_0) ? UCX_DISPLAY_ORIENTATION_90 : UCX_DISPLAY_ORIENTATION_0);
							break;
					}
				/*case SDL_MOUSEBUTTONDOWN:
					if (event.button.which != SDL_TOUCH_MOUSEID)
					{
						uint16_t x,y;
						x = event.button.x / DISPLAY_ZOOM;
						y = event.button.y / DISPLAY_ZOOM;
						BaReCo_touchdown(BaReCo_getActualX(x, y), BaReCo_getActualY(x, y));
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.which != SDL_TOUCH_MOUSEID)
					{
						uint16_t x,y;
						x = event.button.x / DISPLAY_ZOOM;
						y = event.button.y / DISPLAY_ZOOM;
						BaReCo_touchup(BaReCo_getActualX(x, y), BaReCo_getActualY(x, y));
					}
					break;
				case SDL_MOUSEMOTION:
					if (event.button.which != SDL_TOUCH_MOUSEID) {
						if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
							uint16_t x,y;
							x = event.button.x / DISPLAY_ZOOM;
							y = event.button.y / DISPLAY_ZOOM;
							BaReCo_touchmove(BaReCo_getActualX(x, y), BaReCo_getActualY(x, y));
						}
					}
					break;*/
			}
		}
		os_loop();
		graphics_render();
	}
}
