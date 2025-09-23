#include <Arduino.h>
#include <TFT_eSPI.h>  // Thư viện màn hình TFT

// Cấu hình chân
const int micPin = 34;  // MAX9814 OUT
TFT_eSPI tft = TFT_eSPI(); // Khởi tạo TFT

// Biến vẽ sóng
int prevY = 0;
int tftWidth, tftHeight;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  // Khởi tạo TFT
  tft.init();
  tft.setRotation(1); // Ngang
  tft.fillScreen(TFT_BLACK);

  tftWidth = tft.width();
  tftHeight = tft.height();
  prevY = tftHeight / 2;
}

void loop() {
  int micValue = analogRead(micPin); // Đọc tín hiệu mic (0-4095)
  Serial.println(micValue);

  // Chuyển giá trị ADC sang pixel TFT (dọc)
  int y = map(micValue, 0, 4095, tftHeight, 0);

  // Vẽ đường nối sóng âm
  tft.drawLine(tftWidth - 2, prevY, tftWidth - 1, y, TFT_GREEN);
  prevY = y;

  // Dịch màn hình sang trái
  tft.scroll(-1, 0);

  delay(5); // Tốc độ vẽ
}