#include <HLM_storage.h>
#include <nvs_flash.h>

uint32_t HLM_storage_read32(const char name[16]) {
	nvs_handle_t my_handle;
	nvs_open("storage", NVS_READWRITE, &my_handle);
	int32_t value = 0; // value will default to 0, if not set yet in NVS
	nvs_get_i32(my_handle, name, &value);
	nvs_close(my_handle);
	return value;
}

bool HLM_storage_exists32(const char name[16])
{
	nvs_handle_t my_handle;
	nvs_open("storage", NVS_READWRITE, &my_handle);
	int32_t value;
	esp_err_t err = nvs_get_i32(my_handle, name, &value);
	nvs_close(my_handle);
	return (err == ESP_OK);
}

bool HLM_storage_write32(const char name[16], uint32_t value)
{
	nvs_handle_t my_handle;
	nvs_open("storage", NVS_READWRITE, &my_handle);
	esp_err_t err = nvs_set_i32(my_handle, name, value);
	nvs_close(my_handle);
	nvs_commit(my_handle);
    return (err == ESP_OK);
}



