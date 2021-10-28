#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_camera.h"

//! update these if they are wrong, but they appear to be correct
#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1  // software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

static const char *TAG = "example:take_picture";

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

    .pixel_format = PIXFORMAT_RGB565,  // YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_QVGA,  // QQVGA-UXGA Dont go above QVGA unless JPEG

    .jpeg_quality = 12,  // 0-63 lower number means higher quality
    .fb_count = 1,  // if more than one, runs in continuous mode (JPEG only)
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

esp_err_t init_camera() {
    /**
     * Initialize the camera
     * 
     * This must be done before you attempt to call get_picture or
     * clear_picture.
     */
    // Initialize the camera
    ESP_LOGI(TAG, "Initializing camera");
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

camera_fb_t get_picture() {
    /**
     * Take a picture
     * 
     * The camera will take a picture and return a pointer to the frame buffer.
     * The frame buffer contains fields for the image data (buf), the size of
     * the image (len, width, height), the pixel data format (format), and the
     * timestamp since boot (timestamp).
     */
    ESP_LOGI(TAG, "Taking picture...");
    camera_fb_t *pic = esp_camera_fb_get();
    return pic;
}

void clear_picture(camera_fb_t *pic) {
    /**
     * Free the memory allocated for the picture.
     * 
     * This function should only be called after the picture has been taken.
     * If taken before, there will be no frame buffer to provide as the `pic`
     * argument.
     * 
     * pic: The picture taken.
     */
    ESP_LOGI(TAG, "Freeing picture...");
    esp_camera_fb_return(pic);
}

// This bit of code should be put somewhere in the main loop.
// void app_main()
// {
//     if(ESP_OK != init_camera()) {
//         return;
//     }

//     while (1)
//     {
//         ESP_LOGI(TAG, "Taking picture...");
//         camera_fb_t *pic = esp_camera_fb_get();

//         // use pic->buf to access the image
//         ESP_LOGI(TAG, "Picture taken! Its size was: %zu bytes", pic->len);
//         esp_camera_fb_return(pic);

//         vTaskDelay(5000 / portTICK_RATE_MS);
//     }
// }
