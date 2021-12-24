#ifndef _LALEMA_GRAPHICS_H_
#define _LALEMA_GRAPHICS_H_

#include <stdint.h>
#include "../ESP32-HUB75-MatrixPanel-I2S-DMA/ESP32-HUB75-MatrixPanel-I2S-DMA.h"

#define GRAPHICS_COLOR_RED 0xF800
#define GRAPHICS_COLOR_GREEN 0x07E0
#define GRAPHICS_COLOR_BLUE 0x001F

#define GRAPHICS_COLOR_RED1 0x0800
#define GRAPHICS_COLOR_RED2 0x1000
#define GRAPHICS_COLOR_RED3 0x2000
#define GRAPHICS_COLOR_RED4 0x4000
#define GRAPHICS_COLOR_RED5 0x8000

#define GRAPHICS_COLOR_GREEN1 0x0020
#define GRAPHICS_COLOR_GREEN2 0x0040
#define GRAPHICS_COLOR_GREEN3 0x0080
#define GRAPHICS_COLOR_GREEN4 0x0100
#define GRAPHICS_COLOR_GREEN5 0x0200
#define GRAPHICS_COLOR_GREEN6 0x0400

#define GRAPHICS_COLOR_BLUE1 0x0001
#define GRAPHICS_COLOR_BLUE2 0x0002
#define GRAPHICS_COLOR_BLUE3 0x0004
#define GRAPHICS_COLOR_BLUE4 0x0008
#define GRAPHICS_COLOR_BLUE5 0x0010

#define GRAPHICS_COLOR_WHITE 0xFFFF

#ifdef __cplusplus
extern "C" {
#endif

void SetupGraphics();

MatrixPanel_I2S_DMA* getGraphics(void);

void drawString(uint8_t x, uint8_t y, char* text, uint16_t color);
void drawImage1bit(const uint8_t x, const uint8_t y, const uint8_t* img_data, uint8_t w, const uint8_t h, const uint16_t color0, const uint16_t color1);

#ifdef __cplusplus
}
#endif

#endif

