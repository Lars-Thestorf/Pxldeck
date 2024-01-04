
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <esp_log.h>
#include <cstring>

#include <nvs_flash.h>

#include <xmegaComm/xmegaComm.h>

#include <graphics.h>
//#include "../impl/Websocketserver/server.h"

#include <HLM_os.h>


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
	
	MatrixPanel_I2S_DMA* gfx = getGraphics();
	
	while(1){
		if (isXmegaCommEstablished()) {
			gfx->flipDMABuffer();
			gfx->clearScreen();
			
			os_loop();
			
			gfx->showDMABuffer();
		} else {
			gfx->drawPixel(0, 31, GRAPHICS_COLOR_RED);
		}
		vTaskDelay(16 / portTICK_PERIOD_MS);
		
		XmegaCommWorker();
		
	}
}

