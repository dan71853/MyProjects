#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <HTTPClient.h>
#include "Arduino.h"
#include <WiFi.h>


//WiFiHelper
bool connectToRouter();
bool getRequest(HTTPClient* http, const char* endPoint);
bool getShowData(String* showData);


#endif  //FUNCTIONS_H