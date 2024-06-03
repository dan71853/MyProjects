#include "functions.h"

#define M_TO_MS 6e7

void setup() {
  Serial.begin(115200);
  delay(1000);
  //Wake up
  Serial.println("Wakeup");

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

  if (outCollection.length() > 0) {
    Serial.println("outCollection");
    Serial.println(outCollection);
  }

  ESP.deepSleep(1 * M_TO_MS);
}

void loop() {
}