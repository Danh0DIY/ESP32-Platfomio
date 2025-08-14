#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

typedef struct _VideoInfo {
    const uint8_t* const* frames;     
    const uint16_t* frames_size;      
    uint16_t num_frames;              
} VideoInfo;

// ======= INCLUDE tất cả video .h =======
#include "video01.h"
#include "video02.h"
#include "video03.h"
#include "video04.h"

// ======= Tự động tạo mảng videoList =======
VideoInfo* videoList[] = {
    &video01, &video02, &video03, &video04
};

const uint8_t NUM_VIDEOS = sizeof(videoList) / sizeof(videoList[0]);

TFT_eSPI tft = TFT_eSPI();

// Callback vẽ ảnh (kiểu dữ liệu đã sửa chuẩn với TJpg_Decoder)
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
    if (x >= tft.width() || y >= tft.height()) return true;  
    tft.pushImage(x, y, w, h, bitmap);
    return true;
}

// Vẽ 1 frame
void drawJPEGFrame(const VideoInfo* video, uint16_t frameIndex) {
    uint8_t* jpg_data = (uint8_t*)pgm_read_ptr(&video->frames[frameIndex]);
    uint16_t jpg_size = pgm_read_word(&video->frames_size[frameIndex]);

    if (!TJpgDec.drawJpg(0, 0, jpg_data, jpg_size)) {
        Serial.printf("❌ Decode failed on frame %d\n", frameIndex);
    }
}

void setup() {
    Serial.begin(115200);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    TJpgDec.setJpgScale(1);
    TJpgDec.setSwapBytes(true);
    TJpgDec.setCallback(tft_output);
}

void loop() {
    if (NUM_VIDEOS == 0) return;

    for (uint8_t v = 0; v < NUM_VIDEOS; v++) {
        VideoInfo* currentVideo = videoList[v];
        for (uint16_t f = 0; f < currentVideo->num_frames; f++) {
            drawJPEGFrame(currentVideo, f);
            delay(30); // Delay giữa các frame
        }
        delay(300); // Delay giữa các video
    }
}
