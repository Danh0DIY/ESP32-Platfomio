#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WiFiUDP udp;
unsigned int localPort = 4210;   // cổng UDP để nhận dữ liệu

#define OUTPUT_PIN 5  // chân xuất tín hiệu

void setup() {
  Serial.begin(115200);

  // Tạo Wi-Fi AP
  WiFi.softAP(ssid, password);
  Serial.print("WiFi AP dang phat: ");
  Serial.println(ssid);
  Serial.print("Dia chi IP: ");
  Serial.println(WiFi.softAPIP());

  // Bắt đầu UDP server
  udp.begin(localPort);
  Serial.printf("Dang lang nghe UDP port %d\n", localPort);

  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incoming[255];
    int len = udp.read(incoming, 255);
    if (len > 0) {
      incoming[len] = '\0';
    }
    Serial.print("Nhan du lieu: ");
    Serial.println(incoming);

    int soNguoi = atoi(incoming); // chuyển chuỗi sang số
    if (soNguoi > 0) {
      Serial.printf("Xuat %d xung ra chan %d\n", soNguoi, OUTPUT_PIN);
      for (int i = 0; i < soNguoi; i++) {
        digitalWrite(OUTPUT_PIN, HIGH);
        delay(200);
        digitalWrite(OUTPUT_PIN, LOW);
        delay(200);
      }
    }
  }
}