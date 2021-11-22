#include "file_manage.h"

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_defs.h"

#ifdef CONFIG_FILE_MANAGE_ENABLE_EXPLORER
#include "explorer.h"
#endif

#include "esp_console.h"

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

const char *TAG = "file_manager";
const char *MOUNT_POINT = "/sdcard";
static sdmmc_card_t* mounted_card;


void file_manage_mount_sdcard(void) {
    sdmmc_card_t* card;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
    };
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = SD_MOSI,
        .miso_io_num = SD_MISO,
        .sclk_io_num = SD_SCLK,
    };

    esp_err_t ret = spi_bus_initialize(host.slot, &bus_cfg, SPI_DMA_CH1);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialise bus.");
        ESP_ERROR_CHECK(ret);
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    // slot_config.gpio_cs = SD_DATA3
    slot_config.host_id = host.slot;

    ret = esp_vfs_fat_sdspi_mount(
        MOUNT_POINT,
        &host,
        &slot_config,
        &mount_config,
        &card);
    mounted_card = card;

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set the"
                " EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.",
                esp_err_to_name(ret));
        }
        ESP_ERROR_CHECK(ret);
    }
    sdmmc_card_print_info(stdout, card);
}

void file_manage_unmount_sdcard(void) {
    esp_vfs_fat_sdmmc_unmount();
}

// int write_test_sdcard(void) {
//     FILE* f = fopen("/sdcard/test.txt", "w");
//     if (f == NULL) {
//         ESP_LOGE(TAG, "Failed to open file for writing");
//         return false;
//     }
//     fprintf(f, "Hello world!\n");
//     fclose(f);
//     return true;
// }

bool file_manage_write_image(
    uint8_t *img_buffer,
    size_t buffer_size,
    const char *filename) {

    char filepath[64] = "/sdcard/";
    strcat(filepath, filename);
    if (mounted_card == NULL) {
        ESP_LOGE(TAG, "Card not mounted");
        return false;
    }
    FILE* f = fopen(filepath, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return false;
    }
    fwrite(img_buffer, 1, buffer_size, f);
    ESP_LOGI(TAG, "Wrote image to /sdcard/test.jpg");
    fclose(f);
    return true;
}

void file_manager_explorer(void) {
    #ifdef CONFIG_FILE_MANAGE_ENABLE_EXPLORER
    file_explorer_init(mounted_card);
    file_explorer_run();
    #endif
}
