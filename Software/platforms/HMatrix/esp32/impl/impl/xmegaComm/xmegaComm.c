
#include "xmegaComm.h"

#include <esp_log.h>
#include <driver/uart.h>
#include <esp_crc.h>
#include <string.h>

typedef struct statusReportData_t {
	uint8_t raw[0];
	uint16_t buttonData;
	uint8_t joystickXAxis;
	uint8_t joystickYAxis;
	uint8_t joystickRXAxis;
	uint8_t joystickRYAxis;
	uint16_t batteryLevel;
	uint8_t checksumH;
	uint8_t checksumL;
} statusReportData_t;

static const char *TAG = "XmCom";
#define BUF_SIZE 256
uint8_t rxbuffer[BUF_SIZE];

statusReportData_t cached_status;

bool CommEstablished = false;
bool isXmegaCommEstablished(void)
{
	return true;
}

void SetupXmegaComm(void)
{
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
	uart_driver_install(UART_NUM_0, BUF_SIZE, BUF_SIZE, 0, NULL, 0);
	//uart_driver_install(UART_NUM_0, BUF_SIZE, BUF_SIZE, 20, &uart0_queue, 0);
	uart_set_rx_timeout(UART_NUM_0, 2);
	
	ESP_LOGI(TAG,"Reconfigured");
}

void XmegaCommWorker(void)
{
	size_t msglength;
	uart_get_buffered_data_len(UART_NUM_0, &msglength);
	if (msglength < sizeof(statusReportData_t))
		return;
	uart_read_bytes(UART_NUM_0, rxbuffer, sizeof(statusReportData_t), 0); //TODO: read all bytes and check for valid packet at end
	uart_flush_input(UART_NUM_0);
	//printf("%.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x\r\n", rxbuffer[0], rxbuffer[1], rxbuffer[2], rxbuffer[3], rxbuffer[4], rxbuffer[5], rxbuffer[6], rxbuffer[7], rxbuffer[8], rxbuffer[9]);
	uint16_t crc = ~esp_crc16_be(0, rxbuffer, sizeof(statusReportData_t));
	//printf(" CRC: %.4x\r\n", crc);
	if (crc == 0) {
		memcpy(&cached_status, rxbuffer, sizeof(statusReportData_t) - 2);
		CommEstablished = true;
	} else {
		ESP_LOGI(TAG, "corrupt message");
		CommEstablished = false;
	}

	return;
}

uint16_t xmegaGetBatteryLevel()
{
	return cached_status.batteryLevel;
}
uint16_t xmegaGetPressedButtons()
{
	return cached_status.buttonData;
}
uint8_t xmegaGetJoystickXAxis()
{
	return cached_status.joystickXAxis;
}
uint8_t xmegaGetJoystickYAxis()
{
	return cached_status.joystickYAxis;
}
uint8_t xmegaGetJoystickRXAxis()
{
	return cached_status.joystickRXAxis;
}
uint8_t xmegaGetJoystickRYAxis()
{
	return cached_status.joystickRYAxis;
}
