#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "FS.h"
#include <LittleFS.h>
#include <Arduino.h>


/**
 * FileSystem
 */
void createDir(fs::FS &fs, const char *path);

void writeFile(fs::FS &fs, const char *path, const char *message);
void readFile(fs::FS &fs, const char *path);


#endif  //FUNCTIONS_H
