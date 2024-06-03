#define USE_HSPI_FOR_EPD
#define ENABLE_GxEPD2_GFX 0
#define HEY_STOP_THAT \
  while (1) \
    ;


#include "functions.h"


#define sizeArray 200
//134400

void drawTestBitmap();
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("setup");

  initEpaper();
  // SPI.begin(sck, miso, mosi, ss); // preset for remapped pins

  printText("ngdxuhjkgfdshjgd shjkdfvsbhjkgd \n fduksfdskhfhkgfgfsg \n fhdkjshfjudhfjdshfjkdfjhx gihj ghj fxvjghvfsd jhg fvjh");
  // drawBitmaps7c300x180();
  hibernateDisplay();
  // draw4x4Bitmap();
  // drawTestBitmap();
  // helloWorld();
  // delay(5000);
  // drawBitmaps400x300();
  // helloWorlda();

  // display.hibernate();
}




void loop() {
  // put your main code here, to run repeatedly:
}
