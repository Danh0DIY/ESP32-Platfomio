#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  int x = touchRead(33);
  tft.drawCentreString(x, 120, 120, 3);
}

void loop() {
  
}