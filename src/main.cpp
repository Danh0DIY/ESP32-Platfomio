#include <WiFi.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("ESP32 dang phat WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP AP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Không cần làm gì thêm
}