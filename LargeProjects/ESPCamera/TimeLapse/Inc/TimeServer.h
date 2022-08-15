#ifndef TimeServer_h
#define TimeServer_h

#include <WiFi.h>

#include "time.h"

void getTimeFromNTP();

void getLocalTimeString(char* buffer);

#endif