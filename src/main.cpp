#include <BleGamepad.h>

// 3 nút: GPIO0, GPIO20, GPIO21
const int btnPins[] = {0, 20, 21};
const int NUM_BTNS = sizeof(btnPins)/sizeof(btnPins[0]);

bool lastBtnState[3] = {0};

BleGamepad bleGamepad("ESP32 Gamepad", "DIY", 100);

void setup() {
  for (int i = 0; i < NUM_BTNS; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
    lastBtnState[i] = digitalRead(btnPins[i]) == LOW;
  }

  bleGamepad.begin();
}

void loop() {
  if (bleGamepad.isConnected()) {
    for (int i = 0; i < NUM_BTNS; i++) {
      bool pressed = (digitalRead(btnPins[i]) == LOW); // LOW = nhấn
      if (pressed != lastBtnState[i]) {
        lastBtnState[i] = pressed;
        int btnIndex = i + 1; // nút số 1,2,3
        if (pressed) bleGamepad.press(btnIndex);
        else         bleGamepad.release(btnIndex);
      }
    }
  }

  delay(5);
}