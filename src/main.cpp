#include <TFT_eSPI.h>
#include <SPI.h>
#include <SD.h>

TFT_eSPI tft = TFT_eSPI();

// ======= CHÂN SD CARD =======
#define SD_CS    27
#define SD_MOSI  13
#define SD_MISO  12
#define SD_CLK   14

SPIClass spiSD(VSPI);  // SPI riêng cho SD

// ======= HÀM HỖ TRỢ =======
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

// ======= HÀM ĐỌC BMP =======
bool drawBmp(const char *filename, int16_t x, int16_t y) {
  File bmpFile;
  int16_t w, h;
  uint8_t bmpDepth;
  uint32_t bmpImageoffset;
  uint32_t rowSize;
  uint8_t sdbuffer[3*20]; // buffer 20 pixel
  uint8_t buffidx = sizeof(sdbuffer);
  boolean flip = true;
  int16_t row, col;

  if ((x >= tft.width()) || (y >= tft.height())) return false;

  bmpFile = SD.open(filename);
  if (!bmpFile) {
    Serial.print("Không mở được file BMP: "); Serial.println(filename);
    return false;
  }

  if (read16(bmpFile) != 0x4D42) { // Kiểm tra "BM"
    Serial.println("Không phải BMP chuẩn 24-bit");
    bmpFile.close();
    return false;
  }

  (void)read32(bmpFile);
  (void)read32(bmpFile);
  bmpImageoffset = read32(bmpFile);
  (void)read32(bmpFile);
  w = read32(bmpFile);
  h = read32(bmpFile);

  if (read16(bmpFile) != 1) {
    bmpFile.close();
    return false;
  }

  bmpDepth = read16(bmpFile);
  (void)read32(bmpFile);

  if (bmpDepth != 24) {
    Serial.println("BMP không phải 24-bit");
    bmpFile.close();
    return false;
  }

  Serial.print("Hiển thị BMP: "); Serial.print(filename);
  Serial.print(" - "); Serial.print(w); Serial.print("x"); Serial.println(h);

  rowSize = (w*3 + 3) & ~3;
  if (h < 0) { h = -h; flip = false; }

  tft.setSwapBytes(true);
  tft.startWrite();
  for (row=0; row<h; row++) {
    uint32_t pos = flip ? bmpImageoffset + (h-1-row)*rowSize : bmpImageoffset + row*rowSize;
    if (bmpFile.position() != pos) {
      bmpFile.seek(pos);
      buffidx = sizeof(sdbuffer);
    }
    for (col=0; col<w; col++) {
      if (buffidx >= sizeof(sdbuffer)) {
        bmpFile.read(sdbuffer, sizeof(sdbuffer));
        buffidx = 0;
      }
      uint8_t b = sdbuffer[buffidx++];
      uint8_t g = sdbuffer[buffidx++];
      uint8_t r = sdbuffer[buffidx++];
      uint16_t color = tft.color565(r,g,b);
      tft.drawPixel(x+col, y+row, color);
    }
  }
  tft.endWrite();
  bmpFile.close();
  return true;
}

// ======= HÀM TÌM FILE BMP =======
String findBmp() {
  File root = SD.open("/");
  File entry;
  while ((entry = root.openNextFile())) {
    String fname = entry.name();
    if (!entry.isDirectory() && fname.endsWith(".bmp")) {
      entry.close();
      return fname;
    }
    entry.close();
  }
  return ""; // không tìm thấy
}

// ======= SETUP =======
void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  delay(1000); // đợi SD card ổn định

  spiSD.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spiSD, 15000000)) {
    Serial.println("The mo roi mi");
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setCursor(10,10);
    tft.println("SD FAIL!");
    return;
  }
  Serial.println("The day roi nha");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(10,10);
  tft.println("SD OK!");

  // Tìm file BMP đầu tiên
  String bmpFile = findBmp();
  if (bmpFile != "") {
    Serial.print("Tìm thấy file BMP: "); Serial.println(bmpFile);
    drawBmp(bmpFile.c_str(), 0, 0);
  } else {
    Serial.println("Không tìm thấy file BMP!");
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(10,30);
    tft.println("Co anh meo mo");
  }
}

void loop() {
  // không làm gì
}