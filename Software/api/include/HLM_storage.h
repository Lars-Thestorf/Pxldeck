#ifndef HLM_STORAGE_H_
#define HLM_STORAGE_H_ 

#include <stdint.h>

bool HLM_storage_write32(const char name[16], uint32_t value);
bool HLM_storage_exists32(const char name[16]);
uint32_t HLM_storage_read32(const char name[16]);

#endif
