#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

int pinX = 34;   // Joystick X
int pinY = 35;   // Joystick Y
int pinSW = 32;  // Joystick SW

int cursorX = 120; 
int cursorY = 120;
int lastX = cursorX;
int lastY = cursorY;

void setup() {
  Serial.begin(115200);
  pinMode(pinSW, INPUT_PULLUP);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // Vẽ con trỏ ban đầu
  tft.fillCircle(cursorX, cursorY, 3, TFT_GREEN);
}

void loop() {
  int xValue = analogRead(pinX); 
  int yValue = analogRead(pinY); 
  int swValue = digitalRead(pinSW);

  // Map ADC -> tọa độ
  cursorX = map(xValue, 0, 4095, 0, 239);
  cursorY = map(yValue, 0, 4095, 0, 239);

  // Xóa con trỏ cũ
  tft.fillCircle(lastX, lastY, 3, TFT_BLACK);

  // Vẽ con trỏ mới
  if (swValue == LOW) {
    tft.fillCircle(cursorX, cursorY, 3, TFT_RED);   // Nhấn = đỏ
  } else {
    tft.fillCircle(cursorX, cursorY, 3, TFT_GREEN); // Không nhấn = xanh
  }

  lastX = cursorX;
  lastY = cursorY;

  delay(30);
}