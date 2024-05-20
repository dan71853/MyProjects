#include "functions.h"
#include "Arduino.h"
#include "router.h"

const char* serverName = "http://192.168.1.14:8000/";

bool connectToRouter() {
  uint32_t timeout = millis();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED || millis() - timeout > 10000) {  //Wait for 10 seconds
    delay(1000);
    Serial.println("Connecting to WiFi...");
    timeout = millis();
  }

  return WiFi.status() == WL_CONNECTED;  //return true if connected
}

bool getRequest(HTTPClient* http, const char* endPoint) {

  if (WiFi.status() == WL_CONNECTED) {
    char url[64];
    strcpy(url, serverName);
    strcat(url, endPoint);
    http->begin(url);            // Specify the URL
    int httpCode = http->GET();  // Make the request
    return httpCode == 200;
  }
  Serial.println("WiFi not connected");
  return false;
}

bool getShowData(String* showData) {
  HTTPClient http;
  if (getRequest(&http, "shows")) {
    *showData = http.getString();
    http.end();
    return true;
  }
  http.end();
  return false;
}