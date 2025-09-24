#include <Arduino.h>
#include <driver/i2s.h>

#define I2S_SAMPLE_RATE   8000          // Tần số lấy mẫu (8kHz cho thử nghiệm)
#define I2S_SAMPLE_BITS   16            // Độ phân giải 16-bit cho loa
#define I2S_READ_LEN      1024
#define I2S_FORMAT        I2S_CHANNEL_FMT_ONLY_LEFT

// Chân kết nối MAX98357A
#define I2S_BCLK  14   // BCLK
#define I2S_LRC   25   // LRCLK
#define I2S_DOUT  27   // DIN

// Chân mic MAX9814 (analog)
#define MIC_PIN   34

// Cấu hình I2S cho phát ra loa
void i2s_config() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_FORMAT,
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
  analogReadResolution(12);     // 12-bit ADC (0-4095)
  analogSetAttenuation(ADC_11db);

  i2s_config();
  Serial.println("Bắt đầu: Thu & phát cùng lúc...");
}

void loop() {
  int16_t sample = analogRead(MIC_PIN) - 2048;  // Lấy mẫu từ mic, bỏ offset
  sample *= 16;  // Khuếch đại tín hiệu (4096 ADC → 16-bit)

  // Gửi ra loa
  size_t bytes_written;
  i2s_write(I2S_NUM_0, (const char*)&sample, sizeof(sample), &bytes_written, portMAX_DELAY);
}