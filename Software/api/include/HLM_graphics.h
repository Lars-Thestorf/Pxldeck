#ifndef HLM_GRAPHICS_H_
#define HLM_GRAPHICS_H_

#include <stdint.h>

class HLM_graphics{
public:
	void init();
	void clear();
	void drawPixel(uint8_t x, uint8_t y, uint16_t color);
	void drawVLine(uint8_t x, uint8_t y, uint8_t height, uint16_t color);
	void render();
};

//void graphics_init();

HLM_graphics* get_graphics();

#endif
