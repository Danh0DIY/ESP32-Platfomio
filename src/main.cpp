#include <SPI.h>
#include <TFT_eSPI.h>  
#include <SD.h>

TFT_eSPI tft = TFT_eSPI(); 

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);

  if (!SD.begin(27)) {   // Chân CS của SD
    Serial.println("SD Card mount failed!");
    return;
  }

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Loading BMP...", 10, 10);

  // Hiển thị BMP từ thẻ SD
  drawBmp("/test.bmp", 0, 0);  
}

void loop() {}


// --- Hàm đọc BMP 24bit ---
#define BUFFPIXEL 20

void drawBmp(const char *filename, int16_t x, int16_t y) {
  File     bmpFile;
  int16_t  bmpWidth, bmpHeight;   
  uint8_t  bmpDepth;              
  uint32_t bmpImageoffset;        
  uint32_t rowSize;               
  uint8_t  sdbuffer[3*BUFFPIXEL]; 
  uint16_t lcdbuffer[BUFFPIXEL];  
  uint8_t  buffidx = sizeof(sdbuffer); 
  boolean  flip    = true;       
  int16_t  w, h, row, col;
  uint32_t pos = 0, startTime = millis();
  uint8_t  r, g, b;
  boolean  goodBmp = false;      
  boolean  first = true;

  if((x >= tft.width()) || (y >= tft.height())) return;

  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found: "); Serial.println(filename);
    return;
  }

  if(read16(bmpFile) == 0x4D42) { // BMP signature
    (void)read32(bmpFile);
    (void)read32(bmpFile);
    bmpImageoffset = read32(bmpFile);
    (void)read32(bmpFile);
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) {
      bmpDepth = read16(bmpFile);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) {
        goodBmp = true;
        rowSize = (bmpWidth * 3 + 3) & ~3;
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) {
          if(flip) pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) {
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer);
          }

          for (col=0; col<w; col++) {
            if (buffidx >= sizeof(sdbuffer)) {
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0;
            }
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[col] = tft.color565(r,g,b);
          }
          tft.pushColors(lcdbuffer, w, first);
          first = false;
        }
      }
    }
  }
  bmpFile.close();
}

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); 
  ((uint8_t *)&result)[1] = f.read(); 
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); 
  ((uint8_t *)&result)[1] = f.read(); 
  ((uint8_t *)&result)[2] = f.read(); 
  ((uint8_t *)&result)[3] = f.read(); 
  return result;
}