#include <Arduino.h>
#include <TFT_eSPI.h>
#include "image.h" // ảnh RGB565

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(1); // ngang
    tft.fillScreen(TFT_BLACK);

    // Vẽ ảnh từ mảng
    tft.pushImage(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, myImage);
}

void loop() {
    // Không làm gì, ảnh hiển thị cố định
}
