#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>

#define I2S_DOUT 27  // DIN
#define I2S_BCLK 14  // BCLK
#define I2S_LRC  25  // LRC
#define I2S_SD   13  // shutdown pin

#define SAMPLE_RATE 16000
#define TONE_FREQ   440   // phát âm A4 = 440Hz

void setup() {
  pinMode(I2S_SD, OUTPUT);
  digitalWrite(I2S_SD, HIGH);  // bật loa

  // Cấu hình I2S cho loa
  const i2s_config_t i2s_config = {
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

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  // Khởi tạo I2S
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);

  Serial.begin(115200);
  Serial.println("Loa test: Phát âm 440Hz");
}

void loop() {
  static double phase = 0.0;
  int16_t samples[256];

  for (int i = 0; i < 256; i++) {
    samples[i] = (int16_t)(sin(phase) * 30000); // tạo sóng sin
    phase += 2.0 * M_PI * TONE_FREQ / SAMPLE_RATE;
    if (phase >= 2.0 * M_PI) phase -= 2.0 * M_PI;
  }

  size_t bytes_written;
  i2s_write(I2S_NUM_0, samples, sizeof(samples), &bytes_written, portMAX_DELAY);
}