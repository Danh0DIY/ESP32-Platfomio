#include <SPI.h>
#include <SD.h>

#define SD_CS 27
SPIClass spiSD(VSPI);

void setup() {
  Serial.begin(115200);
  delay(500);

  // Khởi tạo SPI riêng cho SD
  spiSD.begin(14, 12, 13, SD_CS); // CLK, MISO, MOSI, CS

  if (!SD.begin(SD_CS, spiSD, 15000000)) {
    Serial.println("SD FAIL!");
    
  }

  Serial.println("SD OK! Liệt kê file root:");

  File root = SD.open("/");
  if(!root){
    Serial.println("Không mở được thư mục root!");
    
  }

  if(!root.isDirectory()){
    Serial.println("Root không phải thư mục!");
    
  }

  File entry;
  while((entry = root.openNextFile())){
    Serial.print(entry.name());
    if(entry.isDirectory()) Serial.println(" <DIR>");
    else Serial.println();
    entry.close();
  }
}

void loop() {
  // Không làm gì
}