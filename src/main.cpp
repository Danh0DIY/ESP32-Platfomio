/* ESP32 SoftAP + Bluetooth + Temperature monitor (Arduino style)
   - Phát WiFi (SoftAP) với web server (port 80) trả trang hiển thị nhiệt độ (auto-refresh)
   - Bật Bluetooth SPP (BluetoothSerial) và gửi nhiệt độ khi có client
   - Đọc nhiệt độ bằng temprature_sens_read()
   - Có cơ chế an toàn: dừng burnCPU khi > MAX_TEMP
*/

#include <WiFi.h>
#include "BluetoothSerial.h"
#include <WebServer.h>   // nếu không có, thay bằng WiFiServer; dùng WebServer cho tiện trong Arduino core

// --- Cấu hình WiFi SoftAP ---
const char* apSSID = "ESP32_AP_Test";
const char* apPass = "12345678"; // nếu muốn mở (không an toàn) set NULL và dùng WiFi.softAP(apSSID);

// --- Bluetooth ---
BluetoothSerial SerialBT;

// --- Web server (port 80) ---
WebServer server(80);

// --- Nhiệt độ ---
extern "C" {
  uint8_t temprature_sens_read();
}
float getChipTemperatureC() {
  // Chuyển từ F sang C theo công thức (temprature_sens_read trả giá trị kiểu F đã được scale)
  // Công thức phổ biến: (raw - 32)/1.8
  return (temprature_sens_read() - 32) / 1.8;
}

// --- Cơ chế an toàn ---
const float MAX_TEMP = 80.0;   // ngưỡng an toàn (°C)
bool overheating = false;

// --- Burn CPU (tùy dùng để làm nóng) ---
void burnCPU() {
  volatile float x = 0.0001;
  for (int i = 0; i < 100000; i++) {
    x = x * 1.00001 + 0.000001;
  }
}

// --- Biến trạng thái ---
unsigned long lastSendBT = 0;
const unsigned long BT_SEND_INTERVAL = 2000; // ms: gửi BT mỗi 2s
unsigned long lastBurnTime = 0;
const unsigned long BURN_INTERVAL = 100; // ms

// --- Web page generator ---
String makePage(float tempC) {
  String page = "<!doctype html><html><head><meta charset='utf-8'>"
                "<meta http-equiv='refresh' content='2'/>" // tự động refresh mỗi 2s
                "<title>ESP32 Temperature</title></head><body>"
                "<h2>ESP32 Temperature Monitor</h2>"
                "<p>Nhiệt độ chip: <strong>";
  page += String(tempC, 2);
  page += " &deg;C</strong></p>";
  page += "<p>Overheating: ";
  page += (overheating ? "YES" : "NO");
  page += "</p>";
  page += "</body></html>";
  return page;
}

// --- HTTP handler ---
void handleRoot() {
  float t = getChipTemperatureC();
  server.send(200, "text/html", makePage(t));
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // 1) Start SoftAP
  Serial.printf("Starting WiFi AP: %s\n", apSSID);
  bool ok;
  if (apPass && strlen(apPass) >= 8) {
    ok = WiFi.softAP(apSSID, apPass);
  } else {
    ok = WiFi.softAP(apSSID); // open AP nếu pass ko đủ dài
  }
  if (!ok) {
    Serial.println("Failed to start AP!");
  } else {
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP started. IP: ");
    Serial.println(apIP);
  }

  // 2) Start Bluetooth SPP
  if (!SerialBT.begin("ESP32_BT")) {
    Serial.println("Failed to start Bluetooth");
  } else {
    Serial.println("Bluetooth started: ESP32_BT");
  }

  // 3) Setup web server routes
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started on port 80");

  // 4) initial status
  overheating = false;
  lastSendBT = millis();
  lastBurnTime = millis();
}

void loop() {
  // Handle web client
  server.handleClient();

  // Read temperature
  float tempC = getChipTemperatureC();

  // Print serial
  Serial.printf("Temp: %.2f °C  | Overheat: %s\n", tempC, overheating ? "YES" : "NO");

  // Send via Bluetooth periodically if connected
  if (SerialBT.hasClient()) {
    unsigned long now = millis();
    if (now - lastSendBT >= BT_SEND_INTERVAL) {
      String msg = "Temp: " + String(tempC, 2) + " C\n";
      SerialBT.print(msg);
      lastSendBT = now;
    }
  }

  // Check overheat
  if (tempC > MAX_TEMP) {
    if (!overheating) {
      Serial.println("⚠️ OVERHEAT detected! Stopping burnCPU.");
      overheating = true;
      // Nếu muốn thêm hành động: ESP.restart() hoặc esp_sleep_start()
    }
  } else {
    // Nếu nhiệt đã hạ xuống thấp hơn ngưỡng - 3 độ -> cho phép chạy lại
    if (overheating && tempC < (MAX_TEMP - 3.0)) {
      Serial.println("Temperature back to safe range. Resume burn if needed.");
      overheating = false;
    }
  }

  // Nếu chưa quá nhiệt thì có thể burn CPU để tạo tải (tuỳ chọn)
  if (!overheating) {
    // Burn trong 1 khoảng nhỏ để tránh chặn server quá lâu
    unsigned long now = millis();
    if (now - lastBurnTime >= BURN_INTERVAL) {
      burnCPU();
      lastBurnTime = now;
    }
  }

  delay(200); // vòng lặp chính, chỉnh nếu cần
}