#include <TFT_eSPI.h>
#include <SPI.h>
#include "image.h"  // file .h bạn gửi (mảng ảnh)

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(1); // Xoay ngang, đổi số nếu cần
  tft.fillScreen(TFT_BLACK);

  // Hiển thị ảnh từ file image.h
  // Giả sử mảng tên là `myImage` và kích thước 160x80
  tft.pushImage(0, 0, 160, 80, video01_jpg_frame_0);
}

void loop() {
  // Không cần làm gì ở đây
}
