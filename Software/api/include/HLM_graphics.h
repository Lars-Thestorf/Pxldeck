#ifndef HLM_GRAPHICS_H_
#define HLM_GRAPHICS_H_

#include <stdint.h>

class HLM_graphics{
public:
	static void init();
	static void clear();
	static void drawPixel(uint8_t x, uint8_t y, uint16_t color);
	static void drawVLine(uint8_t x, uint8_t y, uint8_t height, uint16_t color);
	static void drawHLine(uint8_t x, uint8_t y, uint8_t width, uint16_t color);
	static void fillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color);
	static void drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color);
	static void drawImage1Bit(const uint8_t x, const uint8_t y, const uint8_t* img_data, uint8_t w, const uint8_t h, const uint16_t color0, const uint16_t color1);
	static void drawImageRaw(const uint8_t x, const uint8_t y, const uint8_t* img_data);
	static void drawText(uint8_t x, uint8_t y, const char* str, uint16_t color);
	static void setBrightness(uint8_t percent);
	static void render();
};

//void graphics_init();

HLM_graphics* get_graphics();

#endif
