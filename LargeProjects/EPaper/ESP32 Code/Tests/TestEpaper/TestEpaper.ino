#define USE_HSPI_FOR_EPD
#define ENABLE_GxEPD2_GFX 0
#define HEY_STOP_THAT \
  while (1) \
    ;

#include "BitmapsCollection/WS_Bitmaps7c300x180.h"
#include "BitmapsCollection/testBitmap.h"

#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
SPIClass hspi(HSPI);

GxEPD2_7C< GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=*/15, /*DC=*/27, /*RST=*/26, /*BUSY=*/25));  // Waveshare 5.65" 7-color

#define sizeArray 7184
//134400

void drawTestBitmap();
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("setup");

  // SPI.begin(sck, miso, mosi, ss); // preset for remapped pins

  hspi.begin(13, 12, 14, 15);  // remap hspi for EPD (swap pins)
  display.epd2.selectSPI(hspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));

  display.init(115200);  // default 10ms reset pulse, e.g. for bare panels with DESPI-C02

  // drawBitmaps7c300x180();

  Serial.println("AAAAAAAAAAA");
  Serial.println(sizeof(testBitMap));
  Serial.println(esp_get_free_heap_size());
  Serial.println(ESP.getFreeHeap());
  Serial.println(uxTaskGetStackHighWaterMark(NULL));

 volatile unsigned char testBitMap2[sizeArray];

  Serial.println(sizeof(int));

  for (int i = 0; i < sizeArray ; i++) {
    testBitMap2[i] = testBitMap[i];
    Serial.print(i);
    Serial.print(' ');
    Serial.println(testBitMap[i]);
  }
  // Serial.println(testBitMap2[4]);
  Serial.println(sizeof(testBitMap));
  Serial.println(esp_get_free_heap_size());
  Serial.println(ESP.getFreeHeap());
  Serial.println(uxTaskGetStackHighWaterMark(NULL));

  HEY_STOP_THAT
  draw4x4Bitmap();
  // drawTestBitmap();
  // helloWorld();
  // delay(5000);
  // drawBitmaps400x300();
  // helloWorlda();

  display.hibernate();
}



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


void loop() {
  // put your main code here, to run repeatedly:
}
