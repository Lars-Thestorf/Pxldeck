#ifndef HLM_STORAGE_H_
#define HLM_STORAGE_H_ 

#include <stdint.h>
#include <stdlib.h>

bool HLM_storage_write32(const char name[16], uint32_t value);
bool HLM_storage_exists32(const char name[16]);
uint32_t HLM_storage_read32(const char name[16]);

bool HLM_storage_write_str(const char name[16], char* value);
bool HLM_storage_read_str(const char name[16],char* value, size_t* length);

#endif
