#include <stdio.h>
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
