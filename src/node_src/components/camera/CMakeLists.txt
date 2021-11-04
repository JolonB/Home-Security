idf_component_register(SRCS "camera.c"
                    INCLUDE_DIRS "."
                    REQUIRES flash esp32-camera nvs_flash) # use PRIV_REQUIRES if flash is only included in .c
