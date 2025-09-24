#include <Arduino.h>
#include <driver/i2s.h>

#define I2S_SAMPLE_RATE   8000     // 8kHz
#define I2S_SAMPLE_BITS   16
#define MIC_PIN           34
#define RECORD_TIME       5        // số giây ghi âm
#define BUFFER_SIZE       (I2S_SAMPLE_RATE * RECORD_TIME)

#define I2S_BCLK  14
#define I2S_LRC   25
#define I2S_DOUT  27

int16_t audioBuffer[BUFFER_SIZE];  // Lưu dữ liệu âm thanh

// Cấu hình I2S cho phát
void i2s_config() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 512,
    .use_apll = false,
    .tx_desc_auto_clear = true
  };

  const i2s_pin_config_t pin_config = {
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
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  i2s_config();
  Serial.println("Sẵn sàng ghi âm...");
}

void loop() {
  // --- Ghi âm ---
  Serial.println("Bắt đầu ghi...");
  for (int i = 0; i < BUFFER_SIZE; i++) {
    int adcValue = analogRead(MIC_PIN) - 2048;  // bỏ offset
    audioBuffer[i] = adcValue * 16;             // chuyển sang 16-bit, khuếch đại
    delayMicroseconds(1000000 / I2S_SAMPLE_RATE);
  }
  Serial.println("Ghi âm xong!");

  delay(1000);

  // --- Phát lại ---
  Serial.println("Phát lại...");
  size_t bytes_written;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    i2s_write(I2S_NUM_0, (const char*)&audioBuffer[i], sizeof(int16_t), &bytes_written, portMAX_DELAY);
  }
  Serial.println("Phát xong!");

  delay(3000); // chờ rồi ghi lại vòng tiếp
}