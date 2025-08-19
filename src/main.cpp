#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <WiFi.h>
#include <WebServer.h>

// ======= Cấu hình nút bấm =======
#define BUTTON_PIN 0 // Chân GPIO0 cho nút bấm (active LOW)
#define HOLD_TIME 2000 // Giữ 2 giây để bật/tắt Wi-Fi
bool wifiEnabled = false; // Trạng thái Wi-Fi
unsigned long buttonPressStart = 0;
bool buttonPressed = false;

// ======= Cấu trúc video =======
typedef struct _VideoInfo {
    const uint8_t* const* frames;
    const uint16_t* frames_size;
    uint16_t num_frames;
} VideoInfo;

// ======= Include video =======
#include "video01.h"
#include "video04.h"
#include "video05.h"
#include "video11.h"
#include "video12.h"
#include "video13.h"
#include "video14.h"

VideoInfo* videoList[] = { &video01, &video04, &video05, &video11, &video12, &video13, &video14 };
const uint8_t NUM_VIDEOS = sizeof(videoList) / sizeof(videoList[0]);

TFT_eSPI tft = TFT_eSPI();

// ======= Giao diện web cải tiến =======
const char* uploadForm = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Tải Ảnh JPEG</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f0f2f5;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      margin: 0;
      color: #333;
    }
    .container {
      background: white;
      padding: 2rem;
      border-radius: 10px;
      box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
      max-width: 500px;
      width: 100%;
      text-align: center;
    }
    h2 {
      color: #1a73e8;
      margin-bottom: 1.5rem;
    }
    input[type="file"], select {
      width: 100%;
      padding: 10px;
      margin: 10px 0;
      border: 1px solid #ddd;
      border-radius: 5px;
      font-size: 16px;
    }
    input[type="submit"], button {
      background-color: #1a73e8;
      color: white;
      border: none;
      padding: 12px 20px;
      margin: 10px 5px;
      border-radius: 5px;
      cursor: pointer;
      font-size: 16px;
      transition: background-color 0.3s;
    }
    input[type="submit"]:hover, button:hover {
      background-color: #1557b0;
    }
    .loading {
      display: none;
      font-size: 14px;
      color: #666;
      margin-top: 10px;
    }
    @media (max-width: 600px) {
      .container {
        margin: 1rem;
        padding: 1rem;
      }
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Tải Ảnh JPEG lên ESP32</h2>
    <form method="POST" action="/upload" enctype="multipart/form-data" onsubmit="showLoading()">
      <input type="file" name="file" accept="image/jpeg" required>
      <input type="submit" value="Tải Ảnh (~70KB)">
    </form>
    <form method="GET" action="/selectVideo">
      <select name="video">
        <option value="0">Video 1</option>
        <option value="1">Video 2</option>
        <option value="2">Video 3</option>
        <option value="3">Video 4</option>
        <option value="4">Video 5</option>
        <option value="5">Video 6</option>
        <option value="6">Video 7</option>
        <option value="7">Video 8</option>
        <option value="8">Video 9</option>
        <option value="9">Video 10</option>
        <option value="10">Video 11</option>
        <option value="11">Video 12</option>
        <option value="12">Video 13</option>
        <option value="13">Video 14</option>
      </select>
      <input type="submit" value="Phát Video">
    </form>
    <button onclick="window.location.href='/resume'">Tiếp Tục Phát Video</button>
    <div id="loading" class="loading">Đang tải...</div>
    <script>
      function showLoading() {
        document.getElementById('loading').style.display = 'block';
      }
    </script>
  </div>
</body>
</html>
)rawliteral";

uint8_t* uploadedImage = nullptr;
size_t uploadedSize = 0;

// ======= Chế độ =======
enum Mode { VIDEO_PLAY, IMAGE_DISPLAY };

// ======= Biến toàn cục =======
static uint8_t currentVideo = 0;  // Video hiện tại
static uint16_t currentFrame = 0; // Khung hình hiện tại
static Mode currentMode = VIDEO_PLAY; // Chế độ hiện tại
static unsigned long lastTime = 0; // Thời gian khung hình cuối cùng
static unsigned long imageDisplayStart = 0; // Thời gian bắt đầu hiển thị ảnh

// ======= Wi-Fi & Web Server =======
const char* ssid = "ESP32_AP";
const char* password = "12345678"; // Nên đổi sang mật khẩu mạnh hơn
WebServer server(80);

// ======= Callback hiển thị TFT =======
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
    tft.pushImage(x, y, w, h, bitmap);
    return true;
}

// ======= Vẽ khung hình JPEG =======
void drawJPEGFrame(const VideoInfo* video, uint16_t frameIndex) {
    uint8_t* jpg_data = (uint8_t*)pgm_read_ptr(&video->frames[frameIndex]);
    uint16_t jpg_size = pgm_read_word(&video->frames_size[frameIndex]);
    TJpgDec.setJpgScale(1);
    TJpgDec.drawJpg(0, 0, jpg_data, jpg_size);
}

// ======= Xử lý web =======
void handleRoot() {
    server.send(200, "text/html", uploadForm);
}

void handleUpload() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        if (uploadedImage) free(uploadedImage);
        uploadedSize = 0;
        uploadedImage = (uint8_t*)malloc(70 * 1024);
        if (!uploadedImage) {
            Serial.println("❌ Không đủ RAM để tải ảnh");
            server.send(500, "text/html", "<h3>Lỗi: Không đủ bộ nhớ!</h3><a href='/'>Quay lại</a>");
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (uploadedImage && (uploadedSize + upload.currentSize <= 70 * 1024)) {
            memcpy(uploadedImage + uploadedSize, upload.buf, upload.currentSize);
            uploadedSize += upload.currentSize;
        } else {
            server.send(400, "text/html", "<h3>Lỗi: File quá lớn!</h3><a href='/'>Quay lại</a>");
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        // Kiểm tra file JPEG hợp lệ
        if (uploadedSize >= 2 && uploadedImage[0] == 0xFF && uploadedImage[1] == 0xD8) {
            currentMode = IMAGE_DISPLAY;
            imageDisplayStart = millis(); // Bắt đầu đếm thời gian hiển thị ảnh
            server.send(200, "text/html", "<h3>Tải ảnh thành công!</h3><a href='/'>Quay lại</a>");
        } else {
            free(uploadedImage);
            uploadedImage = nullptr;
            uploadedSize = 0;
            server.send(400, "text/html", "<h3>Lỗi: File không phải JPEG!</h3><a href='/'>Quay lại</a>");
        }
    }
}

void handleResume() {
    currentMode = VIDEO_PLAY;
    server.send(200, "text/html", "<h3>Đã tiếp tục phát video</h3><a href='/'>Quay lại</a>");
}

void handleSelectVideo() {
    if (server.hasArg("video")) {
        currentVideo = server.arg("video").toInt() % NUM_VIDEOS;
        currentFrame = 0;
        currentMode = VIDEO_PLAY;
    }
    server.send(200, "text/html", "<h3>Đã chọn video!</h3><a href='/'>Quay lại</a>");
}

// ======= Điều khiển Wi-Fi =======
void toggleWiFi() {
    if (wifiEnabled) {
        // Tắt Wi-Fi và server
        server.close();
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_OFF);
        wifiEnabled = false;
        Serial.println("Wi-Fi đã tắt");
        if (uploadedImage) {
            free(uploadedImage);
            uploadedImage = nullptr;
            uploadedSize = 0;
            currentMode = VIDEO_PLAY; // Quay lại chế độ video
        }
        tft.fillScreen(TFT_BLACK); // Xóa màn hình
    } else {
        // Bật Wi-Fi và server
        WiFi.softAP(ssid, password);
        if (WiFi.softAPIP()) {
            Serial.println("Điểm truy cập khởi động: ESP32_AP");
            server.on("/", handleRoot);
            server.on("/upload", HTTP_POST, []() {}, handleUpload);
            server.on("/resume", handleResume);
            server.on("/selectVideo", handleSelectVideo);
            server.begin();
            wifiEnabled = true;
            Serial.println("Server web đã khởi động");
        } else {
            Serial.println("❌ Không thể khởi động điểm truy cập");
        }
    }
}

// ======= Setup =======
void setup() {
    Serial.begin(115200);
    
    // Cấu hình nút bấm
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Active LOW (nút kéo xuống GND)

    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    TJpgDec.setJpgScale(1);
    TJpgDec.setSwapBytes(true);
    TJpgDec.setCallback(tft_output);
}

// ======= Vòng lặp chính =======
void loop() {
    // Xử lý nút bấm để bật/tắt Wi-Fi
    if (digitalRead(BUTTON_PIN) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            buttonPressStart = millis();
        }
        if (buttonPressed && (millis() - buttonPressStart >= HOLD_TIME)) {
            toggleWiFi();
            buttonPressed = false; // Reset để tránh lặp lại
            delay(200); // Chống rung nút đơn giản
        }
    } else {
        buttonPressed = false;
    }

    // Xử lý server web nếu Wi-Fi bật
    if (wifiEnabled) {
        static unsigned long lastServerCheck = 0;
        if (millis() - lastServerCheck >= 50) { // Tăng lên 50ms để tối ưu
            server.handleClient();
            lastServerCheck = millis();
        }
    }

    if (currentMode == IMAGE_DISPLAY && uploadedImage) {
        TJpgDec.drawJpg(0, 0, uploadedImage, uploadedSize);
        if (millis() - imageDisplayStart >= 30000) { // Quay lại sau 30 giây
            currentMode = VIDEO_PLAY;
            imageDisplayStart = 0;
        }
        delay(50); // Độ trễ nhỏ để server phản hồi nhanh
        return;
    }

    // Phát video
    if (currentMode == VIDEO_PLAY && NUM_VIDEOS > 0 && millis() - lastTime >= 30) {
        VideoInfo* currentVideoPtr = videoList[currentVideo];
        unsigned long frameStart = millis();
        drawJPEGFrame(currentVideoPtr, currentFrame);
        unsigned long renderTime = millis() - frameStart;
        if (renderTime < 30) delay(30 - renderTime); // Ổn định tốc độ khung hình
        currentFrame++;
        lastTime = millis();
        if (currentFrame >= currentVideoPtr->num_frames) {
            currentFrame = 0;
            currentVideo = (currentVideo + 1) % NUM_VIDEOS;
        }
    }
}