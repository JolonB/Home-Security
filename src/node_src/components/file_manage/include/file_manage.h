#ifndef COMPONENTS_FILE_MANAGE_FILE_MANAGE_H_
#define COMPONENTS_FILE_MANAGE_FILE_MANAGE_H_

#include "sdmmc_cmd.h"  // this may not be needed
#include "driver/sdspi_host.h"
#include "esp_camera.h"

#define SD_DATA2 GPIO_NUM_12
#define SD_DATA3 GPIO_NUM_13  // CD
#define SD_CMD GPIO_NUM_15  // CS/MOSI
#define SD_CLK GPIO_NUM_14  // SCLK
#define SD_DATA0 GPIO_NUM_2  // MISO
#define SD_DATA1 GPIO_NUM_4

#define SD_MOSI GPIO_NUM_15
#define SD_MISO GPIO_NUM_2
#define SD_SCLK GPIO_NUM_14
#define SD_CS GPIO_NUM_13

void file_manage_mount_sdcard(void);
void file_manage_unmount_sdcard(void);
bool file_manage_write_image(uint8_t*, size_t, const char*);
void file_manager_explorer(void);

#endif  // COMPONENTS_FILE_MANAGE_FILE_MANAGE_H_
