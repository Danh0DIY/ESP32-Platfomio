#include <WiFi.h>

#define OUT_PIN 5   // chân xuất tín hiệu

long lastRSSI = -100;
unsigned long lastTrigger = 0;

int selectedNetwork = -1;
bool wifiConnected = false;

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);

  // Quét Wi-Fi
  Serial.println("Dang quet WiFi...");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("Khong tim thay WiFi nao!");
    return;
  }

  Serial.printf("Tim thay %d WiFi:\n", n);
  for (int i = 0; i < n; i++) {
    Serial.printf("%d: %s (RSSI %d)\n", i, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
  }

  Serial.println("Nhap so thu tu WiFi ban muon ket noi:");
}

void loop() {
  // Nếu chưa chọn mạng
  if (selectedNetwork == -1) {
    if (Serial.available()) {
      selectedNetwork = Serial.parseInt();
      if (selectedNetwork >= 0 && selectedNetwork < WiFi.scanNetworks()) {
        Serial.printf("Ban da chon: %s\n", WiFi.SSID(selectedNetwork).c_str());
        Serial.println("Nhap mat khau:");
      } else {
        Serial.println("So thu tu khong hop le, nhap lai!");
        selectedNetwork = -1;
      }
    }
    return;
  }

  // Nếu đã chọn mạng nhưng chưa nhập mật khẩu
  static String password = "";
  if (!wifiConnected && Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      Serial.printf("Dang ket noi toi %s ...\n", WiFi.SSID(selectedNetwork).c_str());
      WiFi.begin(WiFi.SSID(selectedNetwork).c_str(), password.c_str());
      int retry = 0;
      while (WiFi.status() != WL_CONNECTED && retry < 20) {
        delay(500);
        Serial.print(".");
        retry++;
      }
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nDa ket noi WiFi!");
        Serial.print("IP cua ESP32: ");
        Serial.println(WiFi.localIP());
        wifiConnected = true;
      } else {
        Serial.println("\nKhong ket noi duoc, vui long thu lai!");
        selectedNetwork = -1;
        password = "";
      }
    } else {
      password += c;
    }
    return;
  }

  // Nếu đã kết nối Wi-Fi thì đo RSSI
  if (wifiConnected) {
    long rssi = WiFi.RSSI();
    Serial.print("RSSI: ");
    Serial.println(rssi);

    int diff = abs(rssi - lastRSSI);

    // Nếu dao động mạnh => phát hiện có người
    if (diff > 6 && millis() - lastTrigger > 1000) {
      Serial.println("⚠️ Phat hien nguoi di qua!");
      
      digitalWrite(OUT_PIN, HIGH);
      delay(200);
      digitalWrite(OUT_PIN, LOW);
      delay(200);

      lastTrigger = millis();
    }

    lastRSSI = rssi;
    delay(200);
  }
}