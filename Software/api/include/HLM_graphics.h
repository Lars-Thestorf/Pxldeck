#ifndef HLM_GRAPHICS_H_
#define HLM_GRAPHICS_H_

#include <stdint.h>

class HLM_graphics{
public:
	void init();
	void clear();
	void drawPixel(uint8_t x, uint8_t y, uint16_t color);
	void drawVLine(uint8_t x, uint8_t y, uint8_t height, uint16_t color);
	void drawHLine(uint8_t x, uint8_t y, uint8_t width, uint16_t color);
	void fillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color);
	void drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color);
	void drawImage1Bit(const uint8_t x, const uint8_t y, const uint8_t* img_data, uint8_t w, const uint8_t h, const uint16_t color0, const uint16_t color1);
	void setBrightness(uint8_t percent);
	void render();
};

//void graphics_init();

HLM_graphics* get_graphics();

#endif
