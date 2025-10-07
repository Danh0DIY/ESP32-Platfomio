#include <WiFi.h>
#include <ArduinoOTA.h>

// WiFi
const char* ssid = "danh";
const char* password = "1234567899";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println("Dang ket noi WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nDa ket noi WiFi!");
  Serial.print("IP cua ESP32: ");
  Serial.println(WiFi.localIP());

  // Cấu hình OTA
  ArduinoOTA.setHostname("esp32_ota"); // tên thiết bị
  ArduinoOTA.setPassword("1234"); // mật khẩu OTA

  // Các callback
  ArduinoOTA.onStart([]() {
    Serial.println("Bat dau cap nhat OTA...");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nHoan thanh!");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Tien do: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Loi [%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Loi xac thuc");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Loi bat dau");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Loi ket noi");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Loi nhan du lieu");
    else if (error == OTA_END_ERROR) Serial.println("Loi ket thuc");
  });

  ArduinoOTA.begin();
  Serial.println("Da san sang OTA!");
}

void loop() {
  ArduinoOTA.handle();
}