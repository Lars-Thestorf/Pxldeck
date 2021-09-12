
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <esp_log.h>
#include <esp_log.h>
#include "driver/uart.h"
#include <cstring>

#include "bootlogo.c"

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

static const char *TAG = "TAGU";
#define BUF_SIZE 1024
static QueueHandle_t uart0_queue;

char mtbuffer[128];
volatile bool msg_recv = false;

extern "C" {
	void app_main(void);
}

void app_main(void)
{
    printf("Hallo Welt");
	ESP_LOGI("TAG","TEST");
	
	/* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
	uart_driver_install(UART_NUM_0, BUF_SIZE, BUF_SIZE, 20, &uart0_queue, 0);
	
	ESP_LOGI("TAG","Reconfigured");

	memset(mtbuffer, 0, 128);
	
	HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
	HUB75_I2S_CFG mxconfig(
		64, // Module width
		32, // Module height
		1, // chain length
		_pins // pin mapping
	);
	mxconfig.driver = HUB75_I2S_CFG::FM6126A;
	mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_20M;
	mxconfig.double_buff = true;
	dma_display = new MatrixPanel_I2S_DMA(mxconfig);
	dma_display->begin();
	dma_display->setPanelBrightness(20);
	dma_display->flipDMABuffer();
	dma_display->clearScreen();
	for (int i = 0; i < 64 * 32; i++) {
		dma_display->drawPixel(i % 64, i/64, ((bootlogo[i/8] >> (i%8)) & 1) ? 0xFFFF : 0);
	}
	dma_display->showDMABuffer();
	while(1){
		if (msg_recv) {
			char c = mtbuffer[0];
			int i = 0;
			dma_display->flipDMABuffer();
			dma_display->clearScreen();
			dma_display->setCursor(0, 0);
			do {
				dma_display->write(c);
				i++;
				c = mtbuffer[i];
			} while(c != 0);
			dma_display->showDMABuffer();
			msg_recv = false;
		}
		vTaskDelay(50 / portTICK_PERIOD_MS);
			
		//ESP_LOGI("TAG","ich versuche da was anzuzeigen");
		
		uart_event_t event;

        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            //bzero(dtmp, RD_BUF_SIZE);
            ESP_LOGI(TAG, "uart event:");
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.*/
                case UART_DATA:
                    ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
                    uart_read_bytes(UART_NUM_0, mtbuffer, event.size, portMAX_DELAY);
					mtbuffer[event.size] = '\0';
					msg_recv = true;
					ESP_LOGI(TAG, "%s", mtbuffer);
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(UART_NUM_0);
                    xQueueReset(uart0_queue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(UART_NUM_0);
                    xQueueReset(uart0_queue);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error");
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        }
		
		
		
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

