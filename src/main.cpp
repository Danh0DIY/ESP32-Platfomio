#include <Arduino.h>
#include "driver/temperature_sensor.h"
#include <WiFi.h>

// ====== Cấu hình WiFi ======
const char* ssid     = "Ten_WiFi";
const char* password = "Mat_khau";

temperature_sensor_handle_t temp_sensor = NULL;

bool overheating = false;  // cờ trạng thái quá nhiệt
const float MAX_TEMP = 80.0; // ngưỡng an toàn (°C)

// Hàm "đốt CPU"
void burnCPU() {
  volatile float x = 0.0001;
  for (int i = 0; i < 100000; i++) {
    x = x * 1.00001 + 0.000001;
  }
}

void setup() {
  Serial.begin(115200);

  // Cài đặt cảm biến nhiệt độ
  temperature_sensor_config_t config = {
    .range_min = 20,
    .range_max = 100
  };
  temperature_sensor_install(&config, &temp_sensor);
  temperature_sensor_enable(temp_sensor);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.print("Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nĐã kết nối WiFi!");
}

void loop() {
  float temp_c;
  temperature_sensor_get_celsius(temp_sensor, &temp_c);

  Serial.print("Nhiệt độ ESP32: ");
  Serial.print(temp_c);
  Serial.println(" °C");

  // ====== Kiểm tra ngưỡng an toàn ======
  if (temp_c > MAX_TEMP) {
    overheating = true;
    Serial.println("⚠️ QUÁ NHIỆT! Ngừng burnCPU để bảo vệ chip.");
    // Có thể chọn 1 trong các hành động sau:
    // ESP.restart();          // reset ESP32
    // esp_deep_sleep_start(); // đưa vào chế độ sleep
    // bật còi, LED cảnh báo...
  }

  // Nếu chưa quá nhiệt thì tiếp tục đốt CPU
  if (!overheating) {
    for (int i = 0; i < 50; i++) {
      burnCPU();
    }
  }

  delay(500);
}