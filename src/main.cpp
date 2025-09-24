#include <Arduino.h>
#include <TFT_eSPI.h>
#include "BluetoothA2DPSource.h"

#define MIC_PIN 34

TFT_eSPI tft = TFT_eSPI();
BluetoothA2DPSource a2dp_source;

int tftWidth, tftHeight;
int prevY = 0;
int x = 0;

// Callback cho A2DP
int32_t get_audio_data(Frame* frame, int32_t frame_count) {
  for (int i = 0; i < frame_count; i++) {
    int adcValue = analogRead(MIC_PIN) - 2048;  
    int16_t sample = adcValue * 16;  // khuếch đại

    frame[i].channel1 = sample;
    frame[i].channel2 = sample;
  }
  return frame_count;
}

void setup() {
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  // TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tftWidth = tft.width();
  tftHeight = tft.height();
  prevY = tftHeight / 2;

  // Bluetooth
  a2dp_source.start("TênLoaBluetooth", get_audio_data);
}

void loop() {
  // Hiển thị sóng đơn giản
  int micValue = analogRead(MIC_PIN);
  int centered = micValue - 2048;
  int y = (tftHeight / 2) - centered / 20;

  if (y < 0) y = 0;
  if (y > tftHeight - 1) y = tftHeight - 1;

  tft.drawLine(x, 0, x, tftHeight, TFT_BLACK);
  tft.drawLine(x, prevY, x, y, TFT_GREEN);
  prevY = y;

  x++;
  if (x >= tftWidth) x = 0;

  delay(5);
}