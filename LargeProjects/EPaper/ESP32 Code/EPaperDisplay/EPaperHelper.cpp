#include "functions.h"

#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
SPIClass hspi(HSPI);
GxEPD2_7C< GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=*/15, /*DC=*/27, /*RST=*/26, /*BUSY=*/25));  // Waveshare 5.65" 7-color


void initEpaper() {
  hspi.begin(13, 12, 14, 15);  // remap hspi for EPD (swap pins)
  display.epd2.selectSPI(hspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));

  display.init(115200);  // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
}


void printText(const char* text){
  Serial.println("Printing text");
  // display.setRotation(1);
  Serial.println("setFont");
  display.setFont(&FreeMonoBold9pt7b);
  Serial.println("setTextColor");
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  Serial.println("getTextBounds");
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;

  Serial.println("setFullWindow");
  display.setFullWindow();
  Serial.println("firstPage");
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



void updateEpaperImage() {

  const uint8_t tempArray[] = {
    0x44,

  };

  display.writeNative2(tempArray, 0, (display.epd2.WIDTH - 300) / 2, (display.epd2.HEIGHT - 180) / 2, 300, 180, false, false, true);
  display.refresh();
  delay(5000);
}

