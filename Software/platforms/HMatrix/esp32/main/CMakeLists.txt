idf_component_register(SRCS "main.cpp"
                    "../ESP32-HUB75-MatrixPanel-I2S-DMA/ESP32-HUB75-MatrixPanel-I2S-DMA.cpp" "../ESP32-HUB75-MatrixPanel-I2S-DMA/ESP32-HUB75-MatrixPanel-leddrivers.cpp" "../ESP32-HUB75-MatrixPanel-I2S-DMA/esp32_i2s_parallel_v2.c" "../GFX/GFX.cpp" "../GFX/glcdfont.c" "xmegaComm/xmegaComm.c" "powerControl/powerControl.c" "playerInput/playerInput.c" "graphics.cpp" "HLM_random_impl.cpp" "HLM_gfx_adapter.cpp"
                    REQUIRES api os
                    INCLUDE_DIRS ".")
