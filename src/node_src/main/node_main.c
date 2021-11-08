/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "image_saver.h"

static const char* TAG = "Main";

void app_main(void) {
    int i = 0;
    mount_sdcard();
    while (true) {
        printf("Hello world!\n");
        ESP_LOGW(TAG, "This is some logging info with an arg %i", i);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        i++;
    }
}
