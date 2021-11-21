#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "camera.h"

static const char *TAG = "example:take_picture";
bool camera_initialised = false;

static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sscb_sda = CAM_PIN_SIOD,
    .pin_sscb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    // XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,  // YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_SXGA,  // QQVGA-UXGA Dont go above QVGA unless JPEG

    .jpeg_quality = 6,  // 0-63 lower number means higher quality
    .fb_count = 1,  // if more than one, runs in continuous mode (JPEG only)
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

/**
 * Initialize the camera
 * 
 * This must be done before you attempt to call get_picture or
 * clear_picture.
 * 
 * @return ESP_OK if successful, ESP_FAIL otherwise
 */
esp_err_t camera_init_camera() {
    if (camera_initialised) {
        ESP_LOGW(TAG, "Camera already initialised");
        return ESP_OK;
    }

    // Initialize the camera
    ESP_LOGI(TAG, "Initialising camera");
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed");
        return err;
    }

    camera_initialised = true;

    return ESP_OK;
}

esp_err_t camera_expose_camera(void) {
    // TODO(jolonb): should try a different approach to check the brightness of
    // image
    // Could try reading image in different format which provides raw pixel
    // values and then checking the brightness of the image
    int i = 0;
    uint8_t *dummy_buffer = NULL;

    // Check that camera has been initialised
    if (!camera_initialised) {
        ESP_LOGW(TAG, "Camera not initialised. Initialise the camera before"
                    " trying to expose it");
        return ESP_FAIL;
    }

    // Expose the camera
    ESP_LOGI(TAG, "Exposing camera");
    // It looks like we need to take 5 pictures with 1 sec delay to get the
    // right exposure
    while (i < 5) {
        camera_get_picture(&dummy_buffer);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        i++;
    }
    free(dummy_buffer);

    return ESP_OK;
}

/**
 * Take a picture and save it to the provided data buffer.
 * 
 * @param data_buffer The buffer to save the picture to.
 * 
 * @return The length of the image.
 */
size_t camera_get_picture(uint8_t **data) {
    // Check that camera has been initialised
    if (!camera_initialised) {
        ESP_LOGW(TAG, "Camera not initialised. Initialise the camera before"
                    " trying to use get images");
        return 0;
    }

    ESP_LOGI(TAG, "Taking picture...");
    // Get the picture and copy it to the data buffer
    camera_fb_t *pic = esp_camera_fb_get();
    *data = (uint8_t*) malloc(sizeof(uint8_t) * pic->len);
    memcpy(*data, pic->buf, pic->len);
    // Return the buffer
    esp_camera_fb_return(pic);
    // Return the length of the image to the caller
    return pic->len;
}

/**
 * Deinitialize the camera
 * 
 * This must be done after you are done with the camera.
 * 
 * @return ESP_OK if successful, ESP_FAIL otherwise.
 */
esp_err_t camera_deinit_camera() {
    if (!camera_initialised) {
        ESP_LOGW(TAG, "Camera not initialised. Trying to deinitialise anyway");
    }

    ESP_LOGI(TAG, "Deinitialising camera");
    esp_err_t err = esp_camera_deinit();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera deinit failed");
        return err;
    }

    camera_initialised = false;
    return ESP_OK;
}
