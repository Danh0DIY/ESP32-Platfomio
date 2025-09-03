#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

const char* udpAddress = "192.168.4.1"; // IP mặc định của ESP32 AP
const int udpPort = 4210;

WiFiUDP udp;

long lastRSSI = -100;
unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Dang ket noi WiFi...");
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

  // Ngưỡng dao động RSSI để coi là có người đi qua
  if (diff > 6 && millis() - lastSend > 1000) {
    Serial.println("⚠️ Phat hien 1 nguoi di qua");
    udp.beginPacket(udpAddress, udpPort);
    udp.print("1");  // gửi "1" = phát hiện 1 người
    udp.endPacket();
    lastSend = millis();
  }

  lastRSSI = rssi;
  delay(200);
}