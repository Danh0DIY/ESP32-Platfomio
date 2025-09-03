#include <WiFi.h>

// Tạo WiFi Access Point từ ESP32
const char* ap_ssid = "ESP32_Detect";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);

  // Khởi tạo Access Point
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("ESP32 da tao WiFi AP:");
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Lấy số lượng client đang kết nối vào ESP32
  int n = WiFi.softAPgetStationNum();
  if (n > 0) {
    Serial.print("So thiet bi dang ket noi: ");
    Serial.println(n);

    // Duyệt danh sách client
    wifi_sta_list_t stationList;
    tcpip_adapter_sta_list_t adapter;
    esp_wifi_ap_get_sta_list(&stationList);
    tcpip_adapter_get_sta_list(&stationList, &adapter);

    for (int i = 0; i < adapter.num; i++) {
      tcpip_adapter_sta_info_t station = adapter.sta[i];

      // Lấy địa chỉ MAC
      Serial.print("Thiet bi ");
      Serial.print(i + 1);
      Serial.print(" MAC: ");
      for (int j = 0; j < 6; j++) {
        Serial.printf("%02X", station.mac[j]);
        if (j < 5) Serial.print(":");
      }

      // Lấy RSSI (độ mạnh tín hiệu Wi-Fi)
      Serial.print("  RSSI: ");
      Serial.println(station.rssi);

      // Nếu RSSI dao động mạnh => có người di chuyển
      static int lastRSSI = 0;
      int diff = abs(station.rssi - lastRSSI);
      if (diff > 5) {
        Serial.println("⚠️ Phat hien chuyen dong / co nguoi!");
      }
      lastRSSI = station.rssi;
    }
  } else {
    Serial.println("Khong co thiet bi ket noi...");
  }

  delay(500);
}