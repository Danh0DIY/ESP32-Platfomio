#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.setCursor(0, 0);
}

void loop() {
  uint16_t camUng = touchRead(33); // đọc cảm ứng
  tft.fillRect(0, 0, 240, 40, TFT_BLACK); // xóa vùng hiển thị cũ
  tft.setCursor(0, 0);
  tft.print("Gia tri: ");
  tft.println(camUng);
  delay(500); // đọc chậm lại 0.5s để dễ nhìn
}