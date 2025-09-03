#include <WiFi.h>

const char* ssid     = "Ten_WiFi_Nha_Ban";   // đổi thành Wi-Fi nhà bạn
const char* password = "Mat_khau_WiFi";      // đổi mật khẩu cho đúng

#define OUT_PIN 5   // Chân xuất tín hiệu

long lastRSSI = -100;
unsigned long lastTrigger = 0;

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);

  // Kết nối Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Dang ket noi WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nDa ket noi!");
}

void loop() {
  long rssi = WiFi.RSSI();
  Serial.print("RSSI: ");
  Serial.println(rssi);

  int diff = abs(rssi - lastRSSI);

  // Nếu dao động lớn hơn ngưỡng => coi như có người đi qua
  if (diff > 6 && millis() - lastTrigger > 1000) {
    Serial.println("⚠️ Phat hien nguoi di qua!");
    
    // Xuất 1 nhịp ra GPIO 5
    digitalWrite(OUT_PIN, HIGH);
    delay(200);
    digitalWrite(OUT_PIN, LOW);
    delay(200);

    lastTrigger = millis();
  }

  lastRSSI = rssi;
  delay(200);
}