#include <WiFi.h>

#define OUT_PIN 5

String ssid = "";
String password = "";
bool connected = false;

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);

  delay(1000);

  Serial.println("=== QUET WiFi ===");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("Khong tim thay WiFi nao.");
  } else {
    Serial.print("Tim thay ");
    Serial.print(n);
    Serial.println(" mang WiFi:");

    for (int i = 0; i < n; ++i) {
      Serial.print(i);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.println(" dBm)");
      delay(10);
    }

    Serial.println("\nNhap so (index) mang WiFi muon ket noi:");
    while (!Serial.available()) delay(100);
    int index = Serial.parseInt();
    ssid = WiFi.SSID(index);

    Serial.print("Ban da chon: ");
    Serial.println(ssid);

    Serial.println("Nhap mat khau WiFi:");
    while (!Serial.available()) delay(100);
    password = Serial.readStringUntil('\n');
    password.trim();

    Serial.print("Dang ket noi toi ");
    Serial.println(ssid);
    WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    connected = true;
    Serial.println("\nDa ket noi WiFi!");
    Serial.print("IP ESP32: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  static long lastRSSI = -100;
  static unsigned long lastTrigger = 0;

  if (connected && WiFi.status() == WL_CONNECTED) {
    long rssi = WiFi.RSSI();
    Serial.print("RSSI: ");
    Serial.println(rssi);

    int diff = abs(rssi - lastRSSI);

    if (diff > 6 && millis() - lastTrigger > 1000) {
      Serial.println("⚠️ Phat hien nguoi di chuyen!");
      
      // Nháy tín hiệu trên GPIO5
      digitalWrite(OUT_PIN, HIGH);
      delay(200);
      digitalWrite(OUT_PIN, LOW);

      lastTrigger = millis();
    }

    lastRSSI = rssi;
  } else {
    Serial.println("Mat ket noi WiFi!");
  }

  delay(500);
}