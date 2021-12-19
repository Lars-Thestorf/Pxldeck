#include <HLM_random.h>
#include <stdlib.h>

uint32_t HLM_random(uint32_t max) {
	return rand() % max;
}
