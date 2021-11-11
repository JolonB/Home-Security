#ifndef COMPONENTS_IMAGESAVER_IMAGESAVER_H_
#define COMPONENTS_IMAGESAVER_IMAGESAVER_H_

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

void saver_test(void);
void image_saver_mount_sdcard(void);
void image_saver_unmount_sdcard(void);
int write_test_sdcard(void);
bool image_saver_write_image(camera_fb_t *fb);

#endif  // COMPONENTS_IMAGESAVER_IMAGESAVER_H_
