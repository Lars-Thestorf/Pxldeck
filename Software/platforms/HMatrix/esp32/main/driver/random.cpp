#include <HLM_random.h>
#include <esp_system.h>

uint32_t HLM_random(uint32_t max) {
	return esp_random() % max;
}
