#include <Arduino.h>
#include "BluetoothA2DPSource.h"

BluetoothA2DPSource a2dp_source;

// Tần số các nốt nhạc (Hz)
const int NOTE_Cc = 262;
const int NOTE_Dd = 294;
const int NOTE_Ee = 330;
const int NOTE_Ff = 349;

int note_frequency = 0;  // nốt hiện tại
int phase = 0;

int touchPin = T5; // GPIO33

// Hàm phát sóng sin
int32_t get_data(Frame* frame, int32_t frame_count) {
  if (note_frequency == 0) {
    for (int i = 0; i < frame_count; i++) {
      frame[i].channel1 = 0;
      frame[i].channel2 = 0;
    }
    return frame_count;
  }

  float step = (2 * M_PI * note_frequency) / 44100.0; // lấy mẫu 44.1kHz
  for (int i = 0; i < frame_count; i++) {
    int16_t sample = (int16_t)(3000 * sin(phase * step));
    frame[i].channel1 = sample;
    frame[i].channel2 = sample;
    phase++;
  }
  return frame_count;
}

void setup() {
  Serial.begin(115200);
  a2dp_source.start("ESP32-Piano", get_data); // kết nối loa bluetooth
}

void loop() {
  int touchVal = touchRead(touchPin);
  Serial.println(touchVal);

  // Chia giá trị cảm ứng ra các nốt
  if (touchVal < 20)       note_frequency = NOTE_Cc;  // Đô
  else if (touchVal < 40)  note_frequency = NOTE_Dd;  // Rê
  else if (touchVal < 60)  note_frequency = NOTE_Ee;  // Mi
  else if (touchVal < 80)  note_frequency = NOTE_Ff;  // Pha
  else                     note_frequency = 0;       // không chạm → im lặng

  delay(50);
}