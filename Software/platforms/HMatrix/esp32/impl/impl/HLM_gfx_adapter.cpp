#include <HLM_graphics.h>
#include "graphics.h"

HLM_graphics* gfx;

void HLM_graphics::init(){
	
}
void HLM_graphics::clear()
{
	
}
void HLM_graphics::drawPixel ( uint8_t x, uint8_t y, uint16_t color )
{
	MatrixPanel_I2S_DMA *gfx = getGraphics();
	gfx->drawPixel(x,y,color);
}
void HLM_graphics::render(){
	
}
void HLM_graphics::drawVLine ( uint8_t x, uint8_t y, uint8_t height, uint16_t color )
{
	MatrixPanel_I2S_DMA *gfx = getGraphics();
	gfx->drawFastVLine(x, y, height, color);
}
void HLM_graphics::drawHLine(uint8_t x, uint8_t y, uint8_t width, uint16_t color)
{
	MatrixPanel_I2S_DMA *gfx = getGraphics();
	gfx->drawFastHLine(x, y, width, color);
}
void HLM_graphics::drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color)
{
	MatrixPanel_I2S_DMA *gfx = getGraphics();
	gfx->drawRect(x, y, width, height, color);
}
void HLM_graphics::fillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color)
{
	MatrixPanel_I2S_DMA *gfx = getGraphics();
	gfx->fillRect(x, y, width, height, color);
}
void HLM_graphics::drawImage1Bit(const uint8_t x, const uint8_t y, const uint8_t* img_data, uint8_t w, const uint8_t h, const uint16_t color0, const uint16_t color1)
{
	drawImage1bit(x, y, img_data, w, h, color0, color1);
}
void HLM_graphics::drawImageRaw(const uint8_t x, const uint8_t y, const uint8_t* img_data)
{
	drawImageraw(x,y,img_data);
}
void HLM_graphics::drawText(uint8_t x, uint8_t y, const char* str, uint16_t color)
{
	drawString(x, y, str, color);
}



HLM_graphics* get_graphics()
{
	return gfx;
}

void HLM_graphics::setBrightness(uint8_t percent)
{
	MatrixPanel_I2S_DMA *gfx = getGraphics();
	gfx->setPanelBrightness(percent);
}
