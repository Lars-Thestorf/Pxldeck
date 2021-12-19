#include <HLM_time.h>
#include <SDL2/SDL.h>

uint64_t get_ms_since_boot() {
	return SDL_GetTicks();
}
