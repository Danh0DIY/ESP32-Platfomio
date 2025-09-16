#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS 5   // Chân CS của module SD (thường nối D5 trên ESP32)

void setup() {
  Serial.begin(115200);
  if (!SD.begin(SD_CS)) {
    Serial.println("⚠ Không khởi động được thẻ nhớ!");
    return;
  }
  Serial.println("✅ Đã khởi động thẻ nhớ thành công.");

  // Thử ghi file
  File file = SD.open("/test.txt", FILE_WRITE);
  if (file) {
    file.println("Xin chao the nho ESP32!");
    file.close();
    Serial.println("✍ Ghi file test.txt thành công.");
  } else {
    Serial.println("❌ Không thể tạo file test.txt");
  }

  // Thử đọc file
  file = SD.open("/test.txt");
  if (file) {
    Serial.println("📂 Nội dung file test.txt:");
    while (file.available()) {
      Serial.write(file.read());
    }
    file.close();
  } else {
    Serial.println("❌ Không thể mở file test.txt để đọc.");
  }
}

void loop() {}