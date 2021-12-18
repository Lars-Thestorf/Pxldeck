#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <HLM_graphics.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_ZOOM 16

SDL_Texture* displaysurface;
SDL_Renderer* renderer;
SDL_Window* win;
//bool horizontal = false;

HLM_graphics* instanz = NULL;

HLM_graphics* get_graphics() {
	return instanz;
}

void HLM_graphics::init(){
	printf("Hallo Finn\n");
};

void HLM_graphics::drawPixel(uint8_t x, u_int8_t y, uint16_t color) {
	graphics_drawpixel(x,y,color);
};
void HLM_graphics::render(){
	graphics_render();
};
void HLM_graphics::drawVLine(uint8_t x, uint8_t y, uint8_t height, uint16_t color){
	for (uint8_t ypos = 0; ypos < height; ypos++) {
		graphics_drawpixel(x,y+ypos,color);
	}
}
void HLM_graphics::clear()
{
	for (uint8_t x = 0; x < DISPLAY_WIDTH; x++) {
		for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++) {
			graphics_drawpixel(x,y,0x0000);
		}
	}
}


void graphics_init_surface(int w, int h){
	displaysurface = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGB565,
			SDL_TEXTUREACCESS_TARGET,
			w,
			h);
}

bool graphics_init(){
	instanz = new HLM_graphics();
	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	win = SDL_CreateWindow(
					"HMatrix emulator",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					DISPLAY_WIDTH*DISPLAY_ZOOM,
					DISPLAY_HEIGHT*DISPLAY_ZOOM,
					0);

	// triggers the program that controls
	// your graphics hardware and sets flags
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
	// creates a renderer to render our images
	renderer = SDL_CreateRenderer(win, -1, render_flags);

	graphics_init_surface(DISPLAY_WIDTH, DISPLAY_HEIGHT);

	return true;
}

bool graphics_uninit() {
	//free(section_buffer);
	return true;
}

void graphics_render() {
	// clears the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draw displaysurface to window
	SDL_RenderCopy(renderer, displaysurface, NULL, NULL);
	// triggers the double buffers
	// for multiple rendering
	SDL_RenderPresent(renderer);

	// calculates to 60 fps
	SDL_Delay(1000 / 60);
}

//Rotating the emulator might get usefull when games have got actual multiplayer capabilitys
//code stolen from Bareco emulator but not jet ported
/*void set_rotated(ucx_display_orientation_e orientation)
{
	current_orientation = orientation;
	SDL_Texture* oldsurface = displaysurface;
	if (orientation == UCX_DISPLAY_ORIENTATION_0) {
		SDL_SetWindowSize(win, DISPLAY_HEIGHT*DISPLAY_ZOOM, DISPLAY_WIDTH*DISPLAY_ZOOM);
		BaReCo_init_surface(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	} else {
		SDL_SetWindowSize(win, DISPLAY_WIDTH*DISPLAY_ZOOM, DISPLAY_HEIGHT*DISPLAY_ZOOM);
		BaReCo_init_surface(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	}
	SDL_SetRenderTarget(renderer, displaysurface);
	SDL_RenderCopy(renderer, oldsurface, NULL, NULL);
	//SDL_RenderCopyEx(renderer, oldsurface, NULL, NULL, 90 , NULL, SDL_FLIP_NONE);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_DestroyTexture(oldsurface);
}*/

//Will be usefull for drawing Images
/*void BaReCo_updateRectangle(display_section_out_t *section_param)
{
	if(section_param->orientation != current_orientation)
	{
		set_rotated(section_param->orientation);
		current_orientation = section_param->orientation;
	}
	SDL_Rect targetrect;
	targetrect.x=section_param->x;
	targetrect.y=section_param->y;
	targetrect.w=section_param->width;
	targetrect.h=section_param->height;
	SDL_UpdateTexture(displaysurface, &targetrect, section_param->buffer, section_param->width*2);
	if(section_param->buffer_ready_func)
	{
		section_param->buffer_ready_func(section_param->buffer,section_param->width*section_param->height);
	}
}*/

void graphics_drawpixel(uint8_t x, uint8_t y, uint16_t color) {
	SDL_Rect targetrect;
	targetrect.x=x;
	targetrect.y=y;
	targetrect.w=1;
	targetrect.h=1;
	SDL_UpdateTexture(displaysurface, &targetrect, &color, 2);
}

void graphics_drawImage1Bit(const uint8_t x, const uint8_t y, const uint8_t* img_data, uint8_t w, const uint8_t h, const uint16_t color0, const uint16_t color1) {
	for (int i = 0; i < w * h; i++) {
		graphics_drawpixel(x + i % w, y + i/w, ((img_data[i/8] >> (i%8)) & 1) ? color1 : color0);
	}
}
