#include "Functions.h"

//  You only need to format LittleFS the first time you run a
//  test or else use the LITTLEFS plugin to create a partition
//  https://github.com/lorol/arduino-esp32littlefs-plugin


String showData = "https://nyaa.si/?page=rss&q=Chiyu+Mahou+no+Machigatta+Tsukaikata+s01e14&s=seeders&o=desc\nhttps://nyaa.si/?page=rss&q=Delicious+in+Dungeon+s01e21&s=seeders&o=desc\nhttps://nyaa.si/?page=rss&q=MEGATON+MUSASHI+s01e01&s=seeders&o=desc\nhttps://nyaa.si/?page=rss&q=The+Faraway+Paladin+s03e01&s=seeders&o=desc\nhttps://nyaa.si/?page=rss&q=Go!+Go!+Loser+Ranger!+s01e07&s=seeders&o=desc\nhttps://nyaa.si/?page=rss&q=Kaiju+s01e07&s=seeders&o=desc\nhttps://nyaa.si/?page=rss&q=Re+Monster+s01e09&s=seeders&o=desc\nhttps://nyaa.si/?page=rss&q=Jobless+Reincarnation+s02e19&s=seeders&o=desc\nhttps://nyaa.si/?page=rss&q=Moonlit+Fantasy+s02e21&s=seeders&o=desc\nhttps://nyaa.si/?page=rss&q=one+piece+1106+1080&s=seeders&o=desc";




void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting");

  if (initFileSystem() == false) {
    //Error, print error message on screen and go to sleep
  }


  // listDir(LittleFS, "/", 1);  // List the directories up to one level beginning at the root directory



  writeFile(showDataFile, showData.c_str());  // Create a hello1.txt file with the content "Hello1"

  checkForShows(showDataFile);  // Read the complete file

  // deleteFile(LittleFS, "/mydir/hello1.txt"); //delete the previously created file
  // removeDir(LittleFS, "/mydir"); //delete the previously created folder
  // listDir(LittleFS, "/", 1); // list all directories to make sure they were deleted

  // writeFile(LittleFS, "/hello.txt", "Hello "); //Create and write a new file in the root directory
  // appendFile(LittleFS, "/hello.txt", "World!\r\n"); //Append some text to the previous file
  // renameFile(LittleFS, "/hello.txt", "/foo.txt"); //Rename the previous file
  // readFile(LittleFS, "/foo.txt"); //Read the file with the new name
  // deleteFile(LittleFS, "/foo.txt"); //Delete the file
  // testFileIO(LittleFS, "/test.txt"); //Testin
  // deleteFile(LittleFS, "/test.txt"); //Delete the file

  Serial.println("Test complete");
}

void loop() {
}