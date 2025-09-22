#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI(); // uses User_Setup.h

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // backlight PWM
  ledcSetup(0, 5000, 8);
  ledcAttachPin(32, 0);
  ledcWrite(0, 255);

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("TFT_eSPI test", 10, 10);
  tft.fillRect(10, 40, 100, 100, TFT_RED);
}

void loop() {
  // simple animation
  static uint16_t x = 0;
  tft.fillCircle(x, 200, 10, TFT_GREEN);
  delay(30);
  tft.fillCircle(x, 200, 10, TFT_BLACK);
  x += 5;
  if (x > tft.width()) x = 0;
}