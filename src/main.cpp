#include <Arduino.h>
#include <TFT_eSPI.h>  // Thư viện TFT

const int micPin = 34;
TFT_eSPI tft = TFT_eSPI();

int tftWidth, tftHeight;
int prevY = 0;
int x = 0;  // Vị trí vẽ hiện tại

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  tftWidth = tft.width();
  tftHeight = tft.height();
  prevY = tftHeight / 2;
}

void loop() {
  int micValue = analogRead(micPin);
  Serial.println(micValue);

  int y = map(micValue, 0, 4095, tftHeight, 0);

  // Xoá cột pixel trước khi vẽ tiếp
  tft.drawLine(x, 0, x, tftHeight, TFT_BLACK);

  // Vẽ sóng âm
  tft.drawLine(x, prevY, x, y, TFT_GREEN);
  prevY = y;

  x++;
  if (x >= tftWidth) x = 0;  // Quay lại đầu màn hình

  delay(5);
}