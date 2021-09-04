
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <esp_log.h>

#define NO_GFX
#include "../ESP32-HUB75-MatrixPanel-I2S-DMA/ESP32-HUB75-MatrixPanel-I2S-DMA.h"

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

extern "C" {
	void app_main(void);
}

void app_main(void)
{
    printf("Hallo Welt");
	ESP_LOGI("TAG","TEST");

	HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
	HUB75_I2S_CFG mxconfig(
		64, // Module width
		32, // Module height
		1, // chain length
		_pins // pin mapping
	);
	mxconfig.driver = HUB75_I2S_CFG::FM6126A;
	mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_10M;
	dma_display = new MatrixPanel_I2S_DMA(mxconfig);
	dma_display->begin();
	while(1){
	ESP_LOGI("TAG","ich versuche da was anzuzeigen");
	dma_display->setPanelBrightness(20);
	dma_display->fillRect(0,0,64,32,255,0,0);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	dma_display->fillRect(0,0,64,32,0,255,0);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	dma_display->fillRect(0,0,64,32,0,0,255);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	dma_display->fillRect(0,0,64,32,0,0,0);
	dma_display->fillRect(0,26,64,5,255,0,0);
	dma_display->fillRect(0,21,64,5,255,127,0);
	dma_display->fillRect(0,16,64,5,255,255,0);
	dma_display->fillRect(0,11,64,5,0,255,0);
	dma_display->fillRect(0,6,64,5,0,0,255);
	dma_display->fillRect(0,1,64,5,255,0,255);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	
	//dma_display->setBrightness8(255);
	dma_display->fillRect(0,0,64,32,0,0,0);
	dma_display->fillRect(25,0,1,32,255,255,255);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	
	dma_display->fillRect(0,0,64,32,255,255,255);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	}
	//dma_display->showDMABuffer();
	//dma_display->fillRect(2,2,10,10,255,255,255);
	//dma_display->showDMABuffer();
	//ESP_LOGI("TAG","ich versuche da was anzuzeigen");
}

