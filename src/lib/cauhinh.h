#define ST7735_DRIVER
#define ST7735_GREENTAB160x80  // chuẩn cho màn hình 160x80 ST7735S

#define TFT_WIDTH  80
#define TFT_HEIGHT 160

#define TFT_CS   5
#define TFT_DC   1
#define TFT_RST  8

#define TFT_MOSI 3
#define TFT_SCLK 4

#define TFT_RGB_ORDER TFT_BGR     // nếu màu sai, thử đổi sang TFT_RGB
#define TFT_INVERSION_ON          // hoặc _OFF nếu bị âm

#define SPI_FREQUENCY  27000000
