#include <Arduino.h>
#include <TFT_eSPI.h>
#include "image.h" // ảnh RGB565 của bạn

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    Serial.println("Bắt đầu khởi tạo TFT_eSPI...");

    tft.init();
    tft.setRotation(1); // xoay ngang
    tft.fillScreen(TFT_BLACK);

    // Hiển thị ảnh từ file .h
    tft.pushImage(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, myImage);

    Serial.println("Đã hiển thị ảnh!");
}

void loop() {
    // Không làm gì
}
