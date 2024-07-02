#include "functions.h"

#define M_TO_MS 6e7

int THRESHOLD_LEFT = 60;   // Sensitivity threshold
int THRESHOLD_RIGHT = 73;  // Sensitivity threshold
touch_pad_t touchPin;      // GPIO pin that triggered the wake-up


void setup() {
  Serial.begin(115200);
  delay(1000);
  //Wake up
  Serial.println("Wakeup");

  touchPin = esp_sleep_get_touchpad_wakeup_status();  // Store which touch sensor was activated from wake-up data
  if (touchPin == TOUCH_PAD_NUM9) {
    Serial.println("Touch detected on GPIO 32, Left button");
  } else if (touchPin == TOUCH_PAD_NUM5) {
    Serial.println("Touch detected on GPIO 12, Right button");
  } else {
    Serial.println("Wakeup not by touchpad");
  }

  touchSleepWakeUpEnable(T9, THRESHOLD_LEFT);
  touchSleepWakeUpEnable(T5, THRESHOLD_RIGHT);

  //Connect to wifi
  if (connectToRouter() == false) {
    Serial.println("Can't connect to router");
    //Show message on display
    ESP.deepSleep(M_TO_MS * 30);  //Sleep for 30 min
  }

  //Init file system
  if (initFileSystem() == false) {
    Serial.println("Can't start file system");
    //Show message on display
    ESP.deepSleep(M_TO_MS * 30);  //Sleep for 30 min
  }

  //Init EPaper
  initEpaper();

  // String outCollection1;
  // isShowOut("https://nyaa.si/?page=rss&q=Demon+Slayer+s04e03&s=seeders&o=desc",&outCollection1);

  // return;


  String showData;

  if (getShowData(&showData)) {
    Serial.println("Got show data");
    writeFile(showDataFile, showData.c_str());
  } else {
    Serial.println("Error, server not running");
    listDir(LittleFS, "/", 1);  // list all directories to make sure they were deleted
  }

  String outCollection;
  checkForShows(showDataFile, &outCollection);

  // if (outCollection.length() > 0) {
  Serial.println("outCollection");
  Serial.println(outCollection);
  printText(outCollection.c_str());
  // }
  hibernateDisplay();

  ESP.deepSleep(60 * M_TO_MS - millis());
}

void loop() {
}