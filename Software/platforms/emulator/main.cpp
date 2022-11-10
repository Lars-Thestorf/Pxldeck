#include <SDL2/SDL.h>
#include <stdio.h>
#include "graphics.h"
#include <HLM_graphics.h>
//#include "../HMatrix/esp32/main/bootlogo.c"
#include <HLM_os.h>
#include <HLM_random.h>
#include "input.h"

int main()
{
    graphics_init();
    graphics_drawpixel(1,1,0xFFFF);
	//graphics_drawImage1Bit(0, 0, bootlogo, 64, 32, 0x0000, 0xFFFF);

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
						case SDLK_UP: setButtonPressed(BUTTON_DU, true); break;
						case SDLK_DOWN: setButtonPressed(BUTTON_DD, true); break;
						case SDLK_RIGHT: setButtonPressed(BUTTON_DR, true); break;
						case SDLK_LEFT: setButtonPressed(BUTTON_DL, true); break;
						case SDLK_d: setButtonPressed(BUTTON_PRIMARY, true); break;
						case SDLK_f: setButtonPressed(BUTTON_COPRIMARY, true); break;
						case SDLK_a: setButtonPressed(BUTTON_SECONDARY, true); break;
						case SDLK_s: setButtonPressed(BUTTON_COSECONDARY, true); break;
						case SDLK_q: setButtonPressed(BUTTON_LSHOULDER, true); break;
						case SDLK_e: setButtonPressed(BUTTON_RSHOULDER, true); break;
						case SDLK_y: setButtonPressed(BUTTON_LMENU, true); break;
						case SDLK_x: setButtonPressed(BUTTON_HOME, true); break;
						case SDLK_c: setButtonPressed(BUTTON_RMENU, true); break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
						case SDLK_UP: setButtonPressed(BUTTON_DU, false); break;
						case SDLK_DOWN: setButtonPressed(BUTTON_DD, false); break;
						case SDLK_RIGHT: setButtonPressed(BUTTON_DR, false); break;
						case SDLK_LEFT: setButtonPressed(BUTTON_DL, false); break;
						case SDLK_d: setButtonPressed(BUTTON_PRIMARY, false); break;
						case SDLK_f: setButtonPressed(BUTTON_COPRIMARY, false); break;
						case SDLK_a: setButtonPressed(BUTTON_SECONDARY, false); break;
						case SDLK_s: setButtonPressed(BUTTON_COSECONDARY, false); break;
						case SDLK_q: setButtonPressed(BUTTON_LSHOULDER, false); break;
						case SDLK_e: setButtonPressed(BUTTON_RSHOULDER, false); break;
						case SDLK_y: setButtonPressed(BUTTON_LMENU, false); break;
						case SDLK_x: setButtonPressed(BUTTON_HOME, false); break;
						case SDLK_c: setButtonPressed(BUTTON_RMENU, false); break;
					}
					break;
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
