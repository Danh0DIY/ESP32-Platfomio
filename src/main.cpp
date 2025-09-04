#define BUZZER_PIN 25  // Chân nối loa/còi thụ động

// Bảng tần số các nốt nhạc piano (C4 = Đô trung)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880

// Bài Happy Birthday (nốt + thời lượng)
// thời lượng: 4 = đen, 8 = móc đơn, 2 = trắng
int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};

int noteDurations[] = {
  4, 8, 4, 4, 4, 2,
  4, 8, 4, 4, 4, 2,
  4, 8, 4, 4, 4, 4, 2,
  4, 8, 4, 4, 4, 2
};

void setup() {
  ledcAttachPin(BUZZER_PIN, 0); // gắn PWM channel 0
}

void loop() {
  for (int thisNote = 0; thisNote < sizeof(melody)/sizeof(melody[0]); thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    ledcWriteTone(0, melody[thisNote]); // phát nốt
    
    delay(noteDuration);

    // Nghỉ giữa các nốt
    ledcWriteTone(0, 0);
    delay(50);
  }
  
  delay(3000); // nghỉ 3s rồi lặp lại
}