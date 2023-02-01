idf_component_register(SRCS "server.c"
                    REQUIRES nvs_flash esp_http_server
                    INCLUDE_DIRS ".")