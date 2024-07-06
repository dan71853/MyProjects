#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "FS.h"
#include <LittleFS.h>
#include <Arduino.h>


/**
 * FileSystem
 */

extern const char *rootDir;
extern const char *testDataFile;

bool initFileSystem();


//Private??
void createDir(const char *path);
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
void printFile(const char *path, const char *message);
void writeFile(const char *path, const uint8_t *buffer, size_t size);

void readFile(const char *path);
void readFileRaw(const char *path);


void initEpaper();
void hibernateDisplay();
void drawBitmaps7c300x180();


#endif  //FUNCTIONS_H
