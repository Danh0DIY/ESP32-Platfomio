#include "BluetoothA2DPSink.h"

BluetoothA2DPSink a2dp_sink;

// Chân I2S nối MAX98357A
#define I2S_BCLK 14   // BCLK
#define I2S_LRC  25   // LRC
#define I2S_DOUT 27   // DIN

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  Serial.begin(115200);
  Serial.println("ESP32 Bluetooth Speaker Starting...");

  // Setup I2S config
  i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_BCLK,
      .ws_io_num = I2S_LRC,
      .data_out_num = I2S_DOUT,
      .data_in_num = I2S_PIN_NO_CHANGE
  };

  a2dp_sink.set_pin_config(pin_config);

  // Bật Bluetooth A2DP sink
  a2dp_sink.start("ESP32_Speaker"); 
  // "ESP32_Speaker" sẽ hiện trên Bluetooth để điện thoại kết nối
}

void loop() {
  // Không cần gì thêm, loop rỗng
}