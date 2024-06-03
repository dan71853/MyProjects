#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "FS.h"
#include <LittleFS.h>
#include <Arduino.h>


/**
 * FileSystem
 */

extern const char *rootDir;
extern const char *showDataFile;

bool initFileSystem();


//Private??
void createDir(const char *path);

void writeFile(const char *path, const char *message);
void checkForShows(const char *path);


#endif  //FUNCTIONS_H
