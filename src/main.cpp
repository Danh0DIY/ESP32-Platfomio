// MAX9814 Test với ESP32
const int micPin = 34;  // Chân OUT của MAX9814

void setup() {
  Serial.begin(115200);          // Bắt đầu Serial Monitor
  analogReadResolution(12);      // ESP32 ADC 12-bit (0-4095)
  analogSetAttenuation(ADC_11db); // Mở rộng dải đo (0-3.3V)
}

void loop() {
  int micValue = analogRead(micPin);  // Đọc giá trị analog
  Serial.println(micValue);           // In ra Serial Monitor
  delay(50);                          // 20 lần/giây
}