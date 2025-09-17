// Chọn driver
#define ST7789_DRIVER

// Độ phân giải màn hình
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// Chân SPI cho ESP32 DevKit V1
#define TFT_MOSI 18
#define TFT_SCLK 5
#define TFT_CS   23   // Chip select
#define TFT_DC   22   // Data/Command
#define TFT_RST  19   // Reset

#define LOAD_GLCD
#define LOAD_FONT2