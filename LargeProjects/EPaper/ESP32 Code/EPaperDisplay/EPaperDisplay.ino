#include "functions.h"

#define M_TO_US 6e7

int THRESHOLD_LEFT = 60;   // Sensitivity threshold
int THRESHOLD_RIGHT = 73;  // Sensitivity threshold
touch_pad_t touchPin;      // GPIO pin that triggered the wake-up


void setup() {
  Serial.begin(115200);
  delay(1000);
  //Wake up
  Serial.println("Wakeup");
  Serial.println(millis());
  Serial.print("Wake cause: ");
  Serial.println(esp_sleep_get_wakeup_cause());
  if (esp_sleep_get_wakeup_cause() == 0) {
    Serial.println("First startup, going to sleep");
    //Sleep for 5 min, wait for PC to turn on
    ESP.deepSleep(M_TO_US * 2);  //Sleep for a few min
  }

  touchPin = esp_sleep_get_touchpad_wakeup_status();  // Store which touch sensor was activated from wake-up data
  if (touchPin == TOUCH_PAD_NUM9) {
    Serial.println("Touch detected on GPIO 32, Left button");
  } else if (touchPin == TOUCH_PAD_NUM5) {
    Serial.println("Touch detected on GPIO 12, Right button");
  } else {
    Serial.println("Wakeup not by touchpad");
  }

  touchSleepWakeUpEnable(T9, THRESHOLD_LEFT); //Remove for now, think it was waking up when it shouldn't
  touchSleepWakeUpEnable(T5, THRESHOLD_RIGHT);

  //Connect to wifi
  if (connectToRouter() == false) {
    Serial.println("Can't connect to router");
    //Show message on display
    ESP.deepSleep(M_TO_US * 30);  //Sleep for 30 min
  }
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  //Init file system
  if (initFileSystem() == false) {
    Serial.println("Can't start file system");
    //Show message on display
    ESP.deepSleep(M_TO_US * 30);  //Sleep for 30 min
  }

  //Init EPaper
  initEpaper();


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

  if (outCollection.length() > 0) {
    Serial.println("outCollection");
    Serial.println(outCollection);
    printText(outCollection.c_str());
  } else {
    Serial.println("No show, updating image");
    getNewImageData();
    updateEpaperImage();
  }
  hibernateDisplay();

  uint32_t awakeTimeMS = millis();
  uint32_t sleepTime = 15 * M_TO_US;
  uint32_t finalTime = sleepTime - awakeTimeMS * 1000;

  Serial.printf("awakeTime: %u ms = %u us,\n sleepTime: %u us, finalTime: %u us = %u ms\n", awakeTimeMS, awakeTimeMS * 1000, sleepTime, finalTime, finalTime / 1000);

  ESP.deepSleep(finalTime);
}

void loop() {
}