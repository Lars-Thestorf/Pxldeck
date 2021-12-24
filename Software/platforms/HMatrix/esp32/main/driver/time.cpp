#include <HLM_time.h>
#include <esp_timer.h>

uint64_t get_ms_since_boot() {
	return esp_timer_get_time() / 1000;
}
