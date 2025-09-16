#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS 5   // chân CS (ESP32 thường dùng GPIO 5)

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("📂 Liệt kê thư mục: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("❌ Không mở được thư mục");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("❌ Không phải thư mục");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("   📁 ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("   📄 ");
      Serial.print(file.name());
      Serial.print("  (");
      Serial.print(file.size());
      Serial.println(" bytes)");
    }
    file = root.openNextFile();
  }
}

void setup() {
  Serial.begin(115200);
  if (!SD.begin(SD_CS)) {
    Serial.println("⚠ Không khởi động được thẻ nhớ!");
    return;
  }
  Serial.println("✅ Đã khởi động thẻ nhớ thành công.");

  // Liệt kê toàn bộ file/thư mục trong root
  listDir(SD, "/", 2);  // levels = 2 nghĩa là lặp tối đa 2 cấp thư mục
}

void loop() {}