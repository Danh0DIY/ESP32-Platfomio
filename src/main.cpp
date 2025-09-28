#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>

const char* ssid     = "danh";
const char* password = "1234567899";

TFT_eSPI tft = TFT_eSPI();

// Hàm lọc bỏ thẻ HTML
String stripHTML(const String &html) {
  String text = "";
  bool insideTag = false;

  for (int i = 0; i < html.length(); i++) {
    char c = html[i];
    if (c == '<') {
      insideTag = true;
      continue;
    }
    if (c == '>') {
      insideTag = false;
      continue;
    }
    if (!insideTag) {
      text += c;
    }
  }
  return text;
}

void setup() {
  Serial.begin(115200);

  // Khởi tạo màn hình
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);

  // Kết nối WiFi
  tft.println("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
  }
  tft.println("\nWiFi connected!");
  delay(500);

  // Gửi HTTP GET
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://vnexpress.net/doc-truyen-online-doc-truyen-tranh-online-app-doc-truyen-online-em-trai-toi-bi-tay-nao-tu-mot-app-doc-truyen-mien-phi-4923289.html");   // Trang test
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);

      // Lọc bỏ HTML
      String cleanText = stripHTML(payload);

      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0);

      // In khoảng 200 ký tự đầu ra màn hình
      for (int i = 0; i < cleanText.length() && i < 200; i++) {
        char c = cleanText[i];
        if (c == '\n' || c == '\r') continue; // bỏ newline
        tft.print(c);
      }
    } else {
      tft.println("HTTP request failed!");
    }
    http.end();
  }
}

void loop() {
  // Không làm gì thêm
}