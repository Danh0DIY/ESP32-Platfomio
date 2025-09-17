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

  // Vẽ hình chữ nhật
  tft.fillRect(1,1,238,78, TFT_BLUE);
  tft.fillRect(1,82,238,78, TFT_BLUE);
  tft.fillRect(1,162,238,78, TFT_BLUE);
  // Vẽ hình tròn
  tft.fillCircle(120, 120, 40, TFT_BLUE);
  
  //chữ
  
  
  tft.drawCentreString("Gai lon hong",120,42,2);
  tft.drawCentreString("Dit gai xinh",120,122,2);
  tft.drawCentreString("Dit gai nga",120,202,2);
  
}

void loop() {

}