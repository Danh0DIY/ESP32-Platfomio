#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  for (int x=0; x <=240; x +=60) {
    for (int y=0; y<=240; y +=60) {
      tft.fillRect(x, y, 30, 30, TFT_WHITE);
    }
  }
}

void loop() {
  
}