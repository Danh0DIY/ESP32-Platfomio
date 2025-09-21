#include <WiFi.h>

// ===== WiFi để tăng tải (optional) =====
const char* ssid     = "Ten_WiFi";
const char* password = "Mat_khau";

// Hàm đọc nhiệt độ từ cảm biến nội bộ của ESP32
extern "C" {
  uint8_t temprature_sens_read();
}

float getChipTemperature() {
  return (temprature_sens_read() - 32) / 1.8;
}

// Ngưỡng nhiệt độ an toàn
const float MAX_TEMP = 80.0;
bool overheating = false;

// Hàm burn CPU (ép chip tính toán liên tục để sinh nhiệt)
void burnCPU() {
  volatile float x = 0.0001;
  for (int i = 0; i < 100000; i++) {
    x = x * 1.00001 + 0.000001;
  }
}

void setup() {
  Serial.begin(115200);

  // Kết nối WiFi để tạo thêm tải
  WiFi.begin(ssid, password);
  Serial.print("Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nĐã kết nối WiFi!");
}

void loop() {
  float temp_c = getChipTemperature();

  Serial.print("Nhiệt độ chip ESP32: ");
  Serial.print(temp_c);
  Serial.println(" °C");

  // Kiểm tra quá nhiệt
  if (temp_c > MAX_TEMP) {
    overheating = true;
    Serial.println("⚠️ QUÁ NHIỆT! Ngừng burnCPU để bảo vệ chip.");
    // Nếu muốn reset khi quá nhiệt thì bỏ comment dòng này:
    // ESP.restart();
  }

  // Nếu chưa quá nhiệt thì tiếp tục burn CPU
  if (!overheating) {
    for (int i = 0; i < 50; i++) {
      burnCPU();
    }
  }

  delay(500);
}