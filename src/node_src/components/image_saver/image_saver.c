#include <stdio.h>
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_defs.h"

#include "image_saver.h"

/**
 * DATA2 - GPIO12
 * CD/DATA3 - GPIO13
 * CMD - GPIO15
 * CLK - GPIO14
 * DATA0 - GPIO2
 * DATA1 - GPIO4
 * 
 * https://web.archive.org/web/20210502090417if_/https://i1.wp.com/randomnerdtutorials.com/wp-content/uploads/2020/03/ESP32-CAM-AI-Thinker-schematic-diagram.png
 * 
 * Use the sdmmc library (from esp-idf/components)to save the image to the sd card.
 */

const char *TAG = "image_saver";
const char *MOUNT_POINT = "/sdcard";
static sdmmc_card_t* mounted_card;

void saver_test(void) {
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    esp_err_t err = sdspi_host_init();
    if (err != ESP_OK) {
        printf("SDSPI host init failed: %s\n", esp_err_to_name(err));
        return;
    }
}

void image_saver_mount_sdcard(void) {
    sdmmc_card_t* card;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
    };
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = SD_MOSI,
        .miso_io_num = SD_MISO,
        .sclk_io_num = SD_SCLK,};

    esp_err_t ret = spi_bus_initialize(host.slot, &bus_cfg, SPI_DMA_CH1);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialise bus.");
        ESP_ERROR_CHECK(ret);
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    // slot_config.gpio_cs = SD_DATA3
    slot_config.host_id = host.slot;

    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    mounted_card = card;

    if(ret != ESP_OK){
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        ESP_ERROR_CHECK(ret);
    }
    sdmmc_card_print_info(stdout, card);
}

void image_saver_unmount_sdcard(void) {
    esp_vfs_fat_sdmmc_unmount();
}

int write_test_sdcard(void) {
    FILE* f = fopen("/sdcard/test.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return false;
    }
    fprintf(f, "Hello world!\n");
    fclose(f);
    return true;
}

bool image_saver_write_image(camera_fb_t *fb) {
    if (mounted_card == NULL) {
        ESP_LOGE(TAG, "Card not mounted");
        return false;
    }
    FILE* f = fopen("/sdcard/test.jpg", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return false;
    }
    fwrite(fb->buf, 1, fb->len, f);
    ESP_LOGI(TAG, "Wrote image to /sdcard/test.jpg");
    fclose(f);
    return true;
}

char* image_saver_read_directory(char* directory_path) {
    if (mounted_card == NULL) {
        ESP_LOGE(TAG, "Card not mounted");
        return NULL;
    }
    DIR* dir = opendir(directory_path);
    if (dir == NULL) {
        ESP_LOGE(TAG, "Failed to open directory");
        return NULL;
    }
    struct dirent* entry;
    char* result = malloc(sizeof(char) * 1024);
    strcpy(result, "");
    while ((entry = readdir(dir)) != NULL) {
        strcat(result, entry->d_name);
        strcat(result, "\n");
    }
    closedir(dir);
    return result;
}
