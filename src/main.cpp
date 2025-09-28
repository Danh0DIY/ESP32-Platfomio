#include <TFT_eSPI.h>
#include <SPI.h>
#include "Fontchu_vn.c"   // font tiếng Việt mày vừa tạo (đặt trong thư mục src hoặc include)

// Khởi tạo TFT
TFT_eSPI tft = TFT_eSPI();

void setup() {
  // Khởi tạo màn hình
  tft.init();
  tft.setRotation(0);   // 0: dọc, 1: ngang, 2: dọc ngược, 3: ngang ngược
  tft.fillScreen(TFT_BLACK);

  // Chọn màu chữ: trắng, nền: đen
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Load font tiếng Việt
  tft.setFreeFont(&Fontchu_vn);

  // Hiển thị thử
  tft.setCursor(10, 40);
  tft.println("Xin chao ESP32 + ST7789!");

  tft.setCursor(10, 80);
  tft.println("Tieng Viet co dau:");

  tft.setCursor(10, 120);
  tft.println("ă â ê ô ơ ư đ");

  tft.setCursor(10, 160);
  tft.println("À Á Ả Ã Ạ Ầ Ấ Ẩ Ẫ Ậ");

  tft.setCursor(10, 200);
  tft.println("È É Ẻ Ẽ Ẹ Ề Ế Ể Ễ Ệ");

  tft.setCursor(10, 240);
  tft.println("Ò Ó Ỏ Õ Ọ Ờ Ớ Ở Ỡ Ợ");

  tft.setCursor(10, 280);
  tft.println("Ù Ú Ủ Ũ Ụ Ừ Ứ Ử Ữ Ự");

  tft.setCursor(10, 320);
  tft.println("Ỳ Ý Ỷ Ÿ Ỵ");
}

void loop() {
  // không cần làm gì thêm
}