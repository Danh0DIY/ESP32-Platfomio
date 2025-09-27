#include <Arduino.h>
#include <WiFi.h>

const char* ssid     = "Redmi 9A";
const char* password = "1234567899";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("🔌 Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");

  // DNS Lookup
  IPAddress serverIP;
  if (WiFi.hostByName("xiaozhi.me", serverIP)) {
    Serial.print("🌍 xiaozhi.me -> ");
    Serial.println(serverIP);
  } else {
    Serial.println("❌ DNS lookup failed!");
  }
}

void loop() {
  // không cần làm gì thêm
}