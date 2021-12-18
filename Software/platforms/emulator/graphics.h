#ifndef HHTV_GRAPHICS_H_
#define HHTV_GRAPHICS_H_

#include <stdint.h>
#include <stdbool.h>

bool graphics_init();

bool graphics_uninit();

void graphics_render();

void graphics_drawpixel(uint8_t x, uint8_t y, uint16_t color);

void graphics_drawImage1Bit(const uint8_t x, const uint8_t y, const uint8_t* img_data, uint8_t w, const uint8_t h, const uint16_t color0, const uint16_t color1);

#endif
