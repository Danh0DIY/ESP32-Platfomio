#include <Arduino.h>

#define BUZZER_PIN 25  // Chân nối loa/còi

// Định nghĩa một số nốt cơ bản (C4–C5)
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880

// Happy Birthday (nốt + nhịp)
int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};

// thời lượng: 4 = nốt đen, 8 = móc đơn, 2 = nốt trắng
int noteDurations[] = {
  4, 8, 4, 4, 4, 2,
  4, 8, 4, 4, 4, 2,
  4, 8, 4, 4, 4, 4, 2,
  4, 8, 4, 4, 4, 2
};

void setup() {
  ledcAttachPin(BUZZER_PIN, 0); // gán chân vào kênh PWM 0
}

void loop() {
  int notes = sizeof(melody) / sizeof(melody[0]);

  for (int thisNote = 0; thisNote < notes; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    ledcWriteTone(0, melody[thisNote]);  // phát nốt
    delay(noteDuration);

    // nghỉ giữa các nốt
    ledcWriteTone(0, 0);
    delay(50);
  }

  delay(3000); // nghỉ 3 giây rồi lặp lại
}