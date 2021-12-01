// #include <stdio.h>
// #include "sdkconfig.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "esp_spi_flash.h"
#include "esp_log.h"

#include "file_manage.h"
// #include "camera.h"
// #include "time.h"

#include "esp_console.h"

static const char *TAG = "main";

void app_main(void) {
  file_manage_mount_sdcard();

  file_manager_explorer();
  // exit(0);
  /*
  uint8_t *image_data = NULL;
  size_t buffer_len;

  // Init file manager and camera
  camera_init_camera();
  file_manage_mount_sdcard();

  // Expose the camera
  camera_expose_camera();

  // Take image
  int64_t start = esp_timer_get_time();
  buffer_len = camera_get_picture(&image_data);
  int64_t end = esp_timer_get_time();
  ESP_LOGI(TAG, "Time taken to take picture: %d milliseconds", \
              (int)((end - start)/1000));
  if (image_data == NULL) {
      ESP_LOGE(TAG, "Failed to get image data");
      return;
  }

  // Save image
  start = esp_timer_get_time();
  file_manage_write_image(image_data, buffer_len, "image.jpg");
  end = esp_timer_get_time();
  ESP_LOGI(TAG, "Time taken to save image: %d milliseconds", \
              (int)((end - start)/1000));

  // Clean up
  free(image_data);
  file_manage_unmount_sdcard();
  camera_deinit_camera();

  file_manager_explorer();
  */
}
