#include <TFT_eSPI.h>
#include "image.h" // ảnh RGB565

TFT_eSPI tft = TFT_eSPI();

void setup() {
    tft.init();
    tft.setRotation(1);
    tft.pushImage(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, myImage);
}

void loop() {}
