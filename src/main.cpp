#include <WiFi.h>
#include <TFT_eSPI.h>

// --- Cấu hình TFT ---
TFT_eSPI tft = TFT_eSPI();
const int screenWidth = 240;
const int screenHeight = 240;

// --- Cấu hình Wi-Fi ---
const char* staSSID = "danh";      // Wi-Fi muốn kết nối
const char* staPassword = "1234567899";

const char* apSSID = "ESP32_AP";       // Wi-Fi phát ra
const char* apPassword = "12345678";   // Mật khẩu AP

void setup() {
  Serial.begin(115200);

  // TFT init
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.drawString("ESP32 WiFi Demo", 10, 10);

  // --- Khởi động Wi-Fi Station ---
  WiFi.mode(WIFI_AP_STA);  // Vừa STA vừa AP
  WiFi.begin(staSSID, staPassword);
  tft.drawString("Connecting to WiFi...", 10, 40);

  // --- Khởi động AP ---
  WiFi.softAP(apSSID, apPassword);
  tft.drawString("AP Started: " + String(apSSID), 10, 70);
}

void loop() {
  // Kiểm tra trạng thái Wi-Fi STA
  if (WiFi.status() == WL_CONNECTED) {
    String ip = WiFi.localIP().toString();
    tft.fillRect(0, 100, screenWidth, 20, TFT_BLACK);
    tft.drawString("STA IP: " + ip, 10, 100);
  } else {
    tft.fillRect(0, 100, screenWidth, 20, TFT_BLACK);
    tft.drawString("Connecting...", 10, 100);
  }

  // Hiển thị số lượng client kết nối AP
  int clients = WiFi.softAPgetStationNum();
  tft.fillRect(0, 130, screenWidth, 20, TFT_BLACK);
  tft.drawString("AP Clients: " + String(clients), 10, 130);

  delay(1000); // Cập nhật mỗi giây
}