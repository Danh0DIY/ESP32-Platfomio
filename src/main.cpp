#include <Arduino.h>
#include <driver/i2s.h>
#include <TFT_eSPI.h>

#define MIC_PIN 34   // MAX9814 OUT
#define SAMPLE_RATE 16000
#define BUFFER_SIZE 16000  // 1 giây (16kHz)

// MAX98357A pins
#define I2S_DOUT 27
#define I2S_BCLK 14
#define I2S_LRC  25

// Buffer lưu dữ liệu âm thanh
int16_t audioBuffer[BUFFER_SIZE];
int bufferIndex = 0;

bool recording = true;  // trạng thái: ghi hay phát

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  // ADC
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  // Màn hình
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);

  // I2S config cho MAX98357A
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void loop() {
  if (recording) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 50);
    tft.println("Recording...");

    // Ghi 1 giây dữ liệu
    for (bufferIndex = 0; bufferIndex < BUFFER_SIZE; bufferIndex++) {
      int raw = analogRead(MIC_PIN);  // 0–4095
      int16_t sample = (raw - 2048) * 16;  // đưa về signed 16-bit
      audioBuffer[bufferIndex] = sample;
      delayMicroseconds(1000000 / SAMPLE_RATE);
    }

    recording = false;  // chuyển sang phát lại
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 50);
    tft.println("Playing...");

    // Phát lại dữ liệu qua loa
    size_t bytes_written;
    i2s_write(I2S_NUM_0, audioBuffer, sizeof(audioBuffer), &bytes_written, portMAX_DELAY);

    delay(1000);  // nghỉ 1 giây rồi ghi lại
    recording = true;
  }
}