#ifndef COMPONENTS_CAMERA_CAMERA_H_
#define COMPONENTS_CAMERA_CAMERA_H_

#include "esp_camera.h"

#define CAM_PIN_PWDN GPIO_NUM_32
#define CAM_PIN_RESET GPIO_NUM_NC
#define CAM_PIN_XCLK GPIO_NUM_0
#define CAM_PIN_SIOD GPIO_NUM_26
#define CAM_PIN_SIOC GPIO_NUM_27

#define CAM_PIN_D7 GPIO_NUM_35
#define CAM_PIN_D6 GPIO_NUM_34
#define CAM_PIN_D5 GPIO_NUM_39
#define CAM_PIN_D4 GPIO_NUM_36
#define CAM_PIN_D3 GPIO_NUM_21
#define CAM_PIN_D2 GPIO_NUM_19
#define CAM_PIN_D1 GPIO_NUM_18
#define CAM_PIN_D0 GPIO_NUM_5
#define CAM_PIN_VSYNC GPIO_NUM_25
#define CAM_PIN_HREF GPIO_NUM_23
#define CAM_PIN_PCLK GPIO_NUM_22

struct image {
  uint8_t *buf;
  size_t len;
};

esp_err_t camera_init_camera(void);
esp_err_t camera_expose_camera(void);
size_t camera_get_picture(uint8_t**);
esp_err_t camera_deinit_camera(void);

#endif  // COMPONENTS_CAMERA_CAMERA_H_
