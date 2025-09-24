#include <Arduino.h>
#include "BluetoothA2DPSink.h"

BluetoothA2DPSink a2dp_sink;

// Cấu hình chân I2S cho MAX98357A
i2s_pin_config_t i2s_cfg = {
    .bck_io_num = 14,   // BCLK
    .ws_io_num = 25,    // LRC
    .data_out_num = 27, // DIN
    .data_in_num = I2S_PIN_NO_CHANGE
};

void setup() {
  Serial.begin(115200);

  // Đặt tên cho thiết bị Bluetooth (hiện trên điện thoại)
  a2dp_sink.set_pin_config(i2s_cfg);
  a2dp_sink.start("ESP32 Loa Bluetooth");

  Serial.println("ESP32 đã bật chế độ Loa Bluetooth, hãy kết nối từ điện thoại!");
}

void loop() {
  // Không cần gì thêm, thư viện xử lý hết
}