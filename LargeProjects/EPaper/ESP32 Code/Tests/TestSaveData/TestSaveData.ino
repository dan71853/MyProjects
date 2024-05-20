#include "Functions.h"

//  You only need to format LittleFS the first time you run a
//  test or else use the LITTLEFS plugin to create a partition
//  https://github.com/lorol/arduino-esp32littlefs-plugin

#define FORMAT_LITTLEFS_IF_FAILED true


const char* rootDir = "/root";
const char* showDataFile = "/root/showData.txt";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting");

  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  // listDir(LittleFS, "/", 1);  // List the directories up to one level beginning at the root directory

  if (!LittleFS.exists(rootDir)) {
    createDir(LittleFS, rootDir);  // Create a mydir folder
  }

  

  writeFile(LittleFS, showDataFile, "E\nfddsfsdff\nfdsz fdasfdsf fs\ndsf dfdf f\nfdsfdsfdfs");  // Create a hello1.txt file with the content "Hello1"

  readFile(LittleFS, showDataFile); // Read the complete file

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