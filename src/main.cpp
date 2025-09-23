#include <driver/i2s.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();  // TFT ST7789

#define I2S_WS   25   // LRC
#define I2S_SD   27   // DIN
#define I2S_SCK  14   // BCLK

#define MIC_PIN  34   // MAX9814 OUT (nếu dùng ADC, còn ở đây ta dùng I2S với MAX9814 thì bỏ)

// Buffer nhỏ để tiết kiệm RAM
const int bufferLen = 256;
int16_t buffer[bufferLen];

// Khởi tạo I2S cho MIC (RX)
void i2s_mic_init() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = bufferLen,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1, // mic input only
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

// Khởi tạo I2S cho Loa (TX)
void i2s_speaker_init() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = bufferLen,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_SD, // output cho loa
    .data_in_num = -1
  };

  i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_1, &pin_config);
}

void setup() {
  Serial.begin(115200);

  // TFT init
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);

  // MIC + Loa init
  i2s_mic_init();
  i2s_speaker_init();

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("MIC -> WAVES -> SPEAKER", 10, 10, 2);
}

void loop() {
  size_t bytesRead;
  i2s_read(I2S_NUM_0, (void*)buffer, bufferLen * sizeof(int16_t), &bytesRead, portMAX_DELAY);

  int samples = bytesRead / sizeof(int16_t);

  // Vẽ sóng âm (120 điểm)
  tft.fillRect(0, 40, 240, 200, TFT_BLACK);
  for (int i = 0; i < 120 && i < samples; i++) {
    int y = map(buffer[i], -32768, 32767, 240, 40);
    tft.drawPixel(i * 2, y, TFT_YELLOW);
  }

  // Phát lại ra loa
  size_t bytesWritten;
  i2s_write(I2S_NUM_1, (const char*)buffer, samples * sizeof(int16_t), &bytesWritten, portMAX_DELAY);
}