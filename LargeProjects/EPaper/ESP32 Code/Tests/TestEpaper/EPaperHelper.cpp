#include "functions.h"
#include "BitmapsCollection/WS_Bitmaps7c300x180.h"
#include "BitmapsCollection/testBitmap.h"

#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
SPIClass hspi(HSPI);
GxEPD2_7C< GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=*/15, /*DC=*/27, /*RST=*/26, /*BUSY=*/25));  // Waveshare 5.65" 7-color


void initEpaper() {
  hspi.begin(13, 12, 14, 15);  // remap hspi for EPD (swap pins)
  display.epd2.selectSPI(hspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));

  display.init(115200);  // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
}


void printText(const char* text) {
  // display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(text);
  } while (display.nextPage());

  display.hibernate();
}

void hibernateDisplay() {
  display.hibernate();
}

//Old


const char HelloWorld[] = "Hello bbbbb!";

void helloWorld() {
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(HelloWorld);
  } while (display.nextPage());

  display.hibernate();
}
const char HelloWorlda[] = "Hello AAAAAAA!";

void helloWorlda() {
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(HelloWorlda, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(HelloWorlda);
  } while (display.nextPage());
}


const uint8_t bitmap4x4[] = {
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
  0x44,
};

void draw4x4Bitmap() {
  display.writeImage(bitmap4x4, 256, 128, 8, 1, false, false, false);
  // display.writeImage(bitmap4x4, 150, 150, 4, 4, false, false, false);
  display.refresh();
  delay(5000);
}

void drawBitmaps7c300x180() {


  display.drawNative(WS_Bitmap7c300x180, 0, (display.epd2.WIDTH - 300) / 2, (display.epd2.HEIGHT - 180) / 2, 300, 180, false, false, true);
  delay(5000);
}



void drawTestBitmap() {
  // for (int i = 0; i < 5000; i++) {
  //   testBitMap[i] = 0x03;
  // }


  display.drawNative(testBitMap, 0, 0, 0, 600, 448, false, false, true);
}
