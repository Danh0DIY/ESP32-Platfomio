#include <TFT_eSPI.h>
#include <driver/i2s.h>

TFT_eSPI tft = TFT_eSPI();

// === Mic MAX9814 (ADC) ===
#define MIC_CHANNEL ADC1_CHANNEL_6   // GPIO34 = ADC1_CH6

// === Loa MAX98357A (I²S) ===
#define I2S_BCLK 14
#define I2S_LRC  25
#define I2S_DOUT 27
#define I2S_SD   13

// === Thông số ghi âm ===
#define SAMPLE_RATE 16000
#define BITS_PER_SAMPLE 16
#define DURATION_SEC 5
#define TOTAL_SAMPLES (SAMPLE_RATE * DURATION_SEC)

int16_t audioBuffer[TOTAL_SAMPLES];

void setupI2S_mic() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_adc_mode(ADC_UNIT_1, MIC_CHANNEL);
}

void setupI2S_speaker() {
  i2s_driver_uninstall(I2S_NUM_0); // gỡ config cũ

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 256,
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
  i2s_zero_dma_buffer(I2S_NUM_0);
}

void setup() {
  Serial.begin(115200);

  // === TFT init ===
  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH); // backlight
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  // === Mic (ADC via I2S) ===
  setupI2S_mic();

  tft.drawString("Recording 5s...", 40, 10, 2);

  // === Ghi âm ===
  size_t bytes_read;
  int16_t sample;
  int x = 0;

  for (int i = 0; i < TOTAL_SAMPLES; i++) {
    i2s_read(I2S_NUM_0, (char *)&sample, sizeof(sample), &bytes_read, portMAX_DELAY);
    audioBuffer[i] = sample;

    // vẽ sóng (tỉ lệ xuống màn hình)
    int y = map(sample, -2000, 2000, 200, 100);
    tft.drawPixel(x, y, TFT_YELLOW);
    x++;
    if (x >= 240) {  // hết chiều ngang
      x = 0;
      tft.fillRect(0, 100, 240, 140, TFT_BLACK);
    }
  }

  // === Loa (I²S DAC) ===
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Playing...", 70, 10, 2);

  setupI2S_speaker();

  size_t bytes_written;
  for (int i = 0; i < TOTAL_SAMPLES; i++) {
    i2s_write(I2S_NUM_0, &audioBuffer[i], sizeof(int16_t), &bytes_written, portMAX_DELAY);
  }

  tft.drawString("Done!", 90, 40, 2);
}

void loop() {
  // không làm gì thêm
}