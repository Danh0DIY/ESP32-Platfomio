#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <WiFi.h>
#include <WebServer.h>

// ======= Video struct =======
typedef struct _VideoInfo {
    const uint8_t* const* frames;     
    const uint16_t* frames_size;      
    uint16_t num_frames;              
} VideoInfo;

// ======= Include video sẵn có =======
#include "video01.h"
#include "video05.h"
#include "video06.h"

VideoInfo* videoList[] = { &video01, &video05, &video06 };
const uint8_t NUM_VIDEOS = sizeof(videoList)/sizeof(videoList[0]);

// ======= TFT & TJpg_Decoder =======
TFT_eSPI tft = TFT_eSPI();

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
    // Center nếu ảnh nhỏ hơn màn hình
    int16_t drawX = x;
    int16_t drawY = y;
    if (w < tft.width()) drawX += (tft.width() - w)/2;
    if (h < tft.height()) drawY += (tft.height() - h)/2;
    tft.pushImage(drawX, drawY, w, h, bitmap);
    return true;
}

void drawJPEGFrame(const VideoInfo* video, uint16_t frameIndex) {
    uint8_t* jpg_data = (uint8_t*)pgm_read_ptr(&video->frames[frameIndex]);
    uint16_t jpg_size = pgm_read_word(&video->frames_size[frameIndex]);
    TJpgDec.setJpgScale(1);
    TJpgDec.drawJpg(0, 0, jpg_data, jpg_size);
}

// ======= Wi-Fi & Web Server =======
const char* ssid = "ESP32_AP";
const char* password = "12345678";
WebServer server(80);

const char* uploadForm = R"rawliteral(
<!DOCTYPE html>
<html>
  <body>
    <h2>Upload ảnh JPEG để hiển thị</h2>
    <form method="POST" action="/upload" enctype="multipart/form-data">
      <input type="file" name="file">
      <input type="submit" value="Upload">
    </form>
  </body>
</html>
)rawliteral";

uint8_t* uploadedImage = nullptr;
size_t uploadedSize = 0;

void handleRoot() {
    server.send(200, "text/html", uploadForm);
}

void handleUpload() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        if (uploadedImage) free(uploadedImage);
        uploadedSize = 0;
        uploadedImage = (uint8_t*)malloc(upload.totalSize);
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        memcpy(uploadedImage + uploadedSize, upload.buf, upload.currentSize);
        uploadedSize += upload.currentSize;
    } else if (upload.status == UPLOAD_FILE_END) {
        // Tự động scale nếu quá lớn
        TJpgDec.setJpgScale(1); // full size
        TJpgDec.drawJpg(0, 0, uploadedImage, uploadedSize);
        server.send(200, "text/html", "<h3>Upload xong!</h3><a href='/'>Back</a>");
    }
}

// ======= Setup =======
void setup() {
    Serial.begin(115200);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    TJpgDec.setSwapBytes(true);
    TJpgDec.setCallback(tft_output);

    WiFi.softAP(ssid, password);
    Serial.println("AP started: ESP32_AP");

    server.on("/", handleRoot);
    server.on("/upload", HTTP_POST, [](){}, handleUpload);
    server.begin();
}

// ======= Loop chính =======
void loop() {
    server.handleClient();

    static uint8_t currentVideo = 0;
    static uint16_t currentFrame = 0;
    static unsigned long lastTime = 0;

    if (uploadedImage) return; // tạm dừng video khi có ảnh upload

    if (millis() - lastTime >= 30) { // ~33 fps
        drawJPEGFrame(videoList[currentVideo], currentFrame);
        currentFrame++;
        lastTime = millis();
        if (currentFrame >= videoList[currentVideo]->num_frames) {
            currentFrame = 0;
            currentVideo = (currentVideo + 1) % NUM_VIDEOS;
        }
    }
}