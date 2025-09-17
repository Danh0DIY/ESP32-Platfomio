#include <TFT_eSPI.h> 
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Khởi tạo TFT

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);  // Xoay màn hình 0-3

  // Xóa màn hình
  tft.fillScreen(TFT_BLACK);

  // In chữ
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 30);
  tft.println("Xin chao ESP32!, du dit con me may, dit gai xinh mup");

  // Vẽ hình chữ nhật
  tft.fillRect(50, 100, 100, 100, TFT_RED);

  // Vẽ hình tròn
  tft.fillCircle(120, 120, 40, TFT_BLUE);
}

void loop() {
  // Đổi màu màn hình liên tục
  tft.fillScreen(random(0xFFFF));
  delay(1000);
}