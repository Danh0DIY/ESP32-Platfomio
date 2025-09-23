#define MIC_PIN 34

void setup() {
  Serial.begin(115200);
}

void loop() {
  int micValue = analogRead(MIC_PIN);  // đọc ADC 0-4095
  Serial.println(micValue);
  delay(50);
}