idf_component_register(SRCS "../ESP32-HUB75-MatrixPanel-I2S-DMA/ESP32-HUB75-MatrixPanel-I2S-DMA.cpp" "../ESP32-HUB75-MatrixPanel-I2S-DMA/ESP32-HUB75-MatrixPanel-leddrivers.cpp" "../ESP32-HUB75-MatrixPanel-I2S-DMA/esp32_i2s_parallel_v2.c" "../GFX/GFX.cpp" "../GFX/glcdfont.c" "xmegaComm/xmegaComm.c" "driver/random.cpp" "driver/time.cpp" "driver/battery.cpp" "driver/playerInput.cpp" "driver/storage.cpp" "graphics.cpp" "HLM_gfx_adapter.cpp" "driver/network.cpp"
                    PRIV_REQUIRES api
                    REQUIRES nvs_flash driver esp_timer esp_wifi
                    INCLUDE_DIRS ".")

#target_link_libraries(${COMPONENT_LIB} INTERFACE "-u os")
