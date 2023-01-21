
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <esp_log.h>
#include <cstring>

#include <nvs_flash.h>

#include <xmegaComm/xmegaComm.h>

#include <graphics.h>

#include <HLM_os.h>
#include "../Websocketserver/server.h"
#include "../Websocketserver/websocket/include/websocket_server.h"

char mtbuffer[128];

extern "C" {
	void app_main(void);
}

void app_main(void)
{
    printf("Hallo Welt");
	ESP_LOGI("TAG","TEST");
	
	esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
	
	SetupXmegaComm();
	
	SetupGraphics();
	
	os_init();

	wifi_setup();
  	xTaskCreate(&server_task,"server_task",3000,NULL,9,NULL);
  	xTaskCreate(&server_handle_task,"server_handle_task",4000,NULL,6,NULL);
	
	MatrixPanel_I2S_DMA* gfx = getGraphics();
	gfx->setPanelBrightness(10);
	
	while(1){
		if (isXmegaCommEstablished()) {
			gfx->flipDMABuffer();
			gfx->clearScreen();
			
			//DrawMainMenu();
			os_loop();
			
			gfx->showDMABuffer();
		} else {
			gfx->drawPixel(0, 31, GRAPHICS_COLOR_RED);
		}
		vTaskDelay(16 / portTICK_PERIOD_MS);
			
		//ESP_LOGI("TAG","ich versuche da was anzuzeigen");
		
		XmegaCommWorker();
		
		/*dma_display->setPanelBrightness(20);
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
		vTaskDelay(3000 / portTICK_PERIOD_MS);*/
	}
	//dma_display->showDMABuffer();
	//dma_display->fillRect(2,2,10,10,255,255,255);
	//dma_display->showDMABuffer();
	//ESP_LOGI("TAG","ich versuche da was anzuzeigen");
}

