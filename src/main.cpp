#include <WiFi.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

#define LED_PIN 8   // GPIO 8

long lastRSSI = -100;
unsigned long lastTrigger = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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
  if (diff > 6 && millis() - lastTrigger > 1000) {
    Serial.println("⚠️ Phat hien 1 nguoi di qua");
    // Nhấp nháy LED = 1 nhịp
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);

    lastTrigger = millis();
  }

  lastRSSI = rssi;
  delay(200);
}