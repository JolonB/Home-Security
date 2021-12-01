idf_component_register(SRCS "file_manage.c" "explorer.c"
                    INCLUDE_DIRS "./include"
                    REQUIRES sdmmc fatfs esp32-camera colortext
                    PRIV_REQUIRES console)