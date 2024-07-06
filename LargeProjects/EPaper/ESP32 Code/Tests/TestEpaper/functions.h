#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "Arduino.h"

/**
 *EPaper
 */
void initEpaper();
void printText(const char *text);

void helloWorld();

void hibernateDisplay();
void drawBitmaps7c300x180();

#endif  //FUNCTIONS_H
