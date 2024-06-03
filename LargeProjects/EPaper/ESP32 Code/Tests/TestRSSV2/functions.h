#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "Arduino.h"
#include "rssRead.hpp"


/*
 * Router
 */
extern const char* ssid;
extern const char* password;

/**
 * RssHelper
 */
// bool isUploaderTrusted(String* title);
// void isShowOut(const char* showUrl);

void readXML(String xml, String* outCollection);

#endif  //FUNCTIONS_H