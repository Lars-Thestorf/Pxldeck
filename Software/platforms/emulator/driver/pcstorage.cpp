#include <HLM_storage.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

bool HLM_storage_write32(const char name[16], uint32_t value) {
	char path[32];
	snprintf(path, 32, "./.storage/%s", name);
	SDL_RWops *io = SDL_RWFromFile(path, "wb");
	if (io != NULL) {
		size_t wrote = SDL_RWwrite(io, &value, 4, 1);
		SDL_RWclose(io);
		if (wrote > 0) {
			return true;
		}
	}
	return false;
}

bool HLM_storage_exists32(const char name[16])
{
	char path[32];
	snprintf(path, 32, "./.storage/%s", name);
	SDL_RWops *io = SDL_RWFromFile(path, "rb");
	if (io != NULL) {
		SDL_RWclose(io);
		return true;
	}
	return false;
}

uint32_t HLM_storage_read32(const char name[16]) {
	char path[32];
	snprintf(path, 32, "./.storage/%s", name);
	SDL_RWops *io = SDL_RWFromFile(path, "rb");
	uint32_t value = 0;
	if (io != NULL) {
		SDL_RWread(io, &value, 4, 1);
		SDL_RWclose(io);
	}
	return value;
}

bool HLM_storage_write_str(const char name[16],char * value){
	char path[32];
	size_t len = strlen(value);
	snprintf(path, 32, "./.storage/%s", name);
	SDL_RWops *io = SDL_RWFromFile(path, "wb");
	if (io != NULL) {
		size_t wrote = SDL_RWwrite(io, value, 1, len);
		SDL_RWclose(io);
		if (wrote > 0) {
			return true;
		}
	}
	return false;
}

bool HLM_storage_read_str(const char name[16],char * value, size_t *length){
	char path[32];
	snprintf(path, 32, "./.storage/%s", name);
	SDL_RWops *io = SDL_RWFromFile(path, "rb");
	if (io != NULL) {
		SDL_RWread(io, value, 1, *length);
		SDL_RWclose(io);
	}
	return value;
}