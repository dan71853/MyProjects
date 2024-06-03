#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <HTTPClient.h>
#include "Arduino.h"
#include <WiFi.h>
#include "rssRead.hpp"
#include <LittleFS.h>


/*
 * Router
 */
extern const char* ssid;
extern const char* password;

/*
 * WiFiHelper
 */
bool connectToRouter();
bool getRequest(HTTPClient* http, const char* endPoint);
bool getShowData(String* showData);


/*
 * FileSystem
 */

extern const char* rootDir;
extern const char* showDataFile;

bool initFileSystem();
void listDir(fs::FS& fs, const char* dirname, uint8_t levels);


//Private??
void createDir(const char* path);

void writeFile(const char* path, const char* message);
void checkForShows(const char* path,String *outCollection);


/*
 * RssHelper
 */
void checkShow(const char* url, String* outCollection);

/**
 *EPaper
 */
void initEpaper();
void printText(const char *text);
void hibernateDisplay();



#endif  //FUNCTIONS_H