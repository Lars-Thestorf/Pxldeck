
#include "graphics.h"

#include "bootlogo.c"

#include <esp_log.h>

// Change these to whatever suits
#define R1_PIN 32
#define G1_PIN 33
#define B1_PIN 27
#define R2_PIN 14
#define G2_PIN 12
#define B2_PIN 13
#define A_PIN 2
#define B_PIN 0
#define C_PIN 4
#define D_PIN 16
#define E_PIN -1 // required for 1/32 scan panels, like 64x64. Any available pin would do, i.e. IO32
#define LAT_PIN 5
#define OE_PIN 18
#define CLK_PIN 17

MatrixPanel_I2S_DMA* dma_display;

static const char *TAG = "Graphics";

void SetupGraphics() {
	HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
	HUB75_I2S_CFG mxconfig(
		64, // Module width
		32, // Module height
		1, // chain length
		_pins // pin mapping
	);
	mxconfig.driver = HUB75_I2S_CFG::FM6126A;
	//mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_10M;
	mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_20M;
	mxconfig.double_buff = true;
	//mxconfig.latch_blanking = 3;
	mxconfig.clkphase = false;
	dma_display = new MatrixPanel_I2S_DMA(mxconfig);
	dma_display->begin();
	dma_display->setPanelBrightness(20);
	dma_display->flipDMABuffer();
	dma_display->clearScreen();
	drawImage1bit(0, 0, bootlogo, 64, 32, 0x0000, 0xFFFF);
	dma_display->showDMABuffer();
	ESP_LOGI(TAG,"Showing boot image");	
}

MatrixPanel_I2S_DMA* getGraphics() {
	return dma_display;
}

void drawImage1bit(const uint8_t x, const uint8_t y, const uint8_t* img_data, uint8_t w, const uint8_t h, const uint16_t color0, const uint16_t color1) {
	for (int i = 0; i < w * h; i++) {
		dma_display->drawPixel(x + i % w, y + i/w, ((img_data[i/8] >> (i%8)) & 1) ? color1 : color0);
	}
}

void drawString(uint8_t x, uint8_t y, char* text) {
	dma_display->setCursor(x, y);
	char c = text[0];
	int i = 0;
	while (c != '\0') {
		dma_display->write(c);
		i++;
		c = text[i];
	}
}
