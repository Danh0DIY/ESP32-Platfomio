#include <Arduino.h>
#include "BluetoothA2DPSource.h"

BluetoothA2DPSource a2dp_source;

const int micPin = 34;   // Mic analog (MAX9814 OUT)
int16_t sample_buffer[256];

// Hàm lấy dữ liệu từ mic để gửi sang loa
int32_t get_audio_data(Frame* data, int32_t frameCount) {
  for (int i = 0; i < frameCount; i++) {
    int micValue = analogRead(micPin);              // đọc ADC 0–4095
    int16_t sample = map(micValue, 0, 4095, -32768, 32767);
    data[i].channel1 = sample;   // kênh trái
    data[i].channel2 = sample;   // kênh phải
  }
  return frameCount;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  Serial.println("Đang scan loa Bluetooth...");

  // Tìm thiết bị Bluetooth khả dụng
  std::vector<BluetoothA2DPSource::BluetoothDevice> devices = a2dp_source.discover();

  if (devices.size() == 0) {
    Serial.println("Không tìm thấy loa nào!");
    return;
  }

  // In ra danh sách để biết MAC
  for (int i = 0; i < devices.size(); i++) {
    Serial.printf("[%d] %s - %s\n", i, devices[i].name.c_str(), devices[i].address.toString().c_str());
  }

  // Ở đây chọn thiết bị đầu tiên
  auto target = devices[0];
  Serial.printf("Kết nối tới: %s (%s)\n", target.name.c_str(), target.address.toString().c_str());

  // Bắt đầu stream mic → loa
  a2dp_source.start(target.name.c_str(), get_audio_data, target.address.toString().c_str());
}

void loop() {
  // Không cần thêm code ở loop
}