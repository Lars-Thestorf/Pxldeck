
#include "pong.h"
#include <esp_log.h>

static const char *TAG = "Pong";

const LaLeMaGame pong_game = {
	"Pong",
	*ponggame_setup,
	*ponggame_loop,
	*ponggame_free
};

void* ponggame_setup() {
	ESP_LOGI(TAG, "PongStart\r\n");
	return nullptr;
}
void ponggame_loop(void* mem) {
	ESP_LOGI(TAG, "PongLoop\r\n");
}
void ponggame_free(void* mem) {
	ESP_LOGI(TAG, "PongFree\r\n");
}
