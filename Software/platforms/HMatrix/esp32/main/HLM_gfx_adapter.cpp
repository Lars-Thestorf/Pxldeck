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
HLM_graphics* get_graphics()
{
	return gfx;
}



