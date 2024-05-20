#include <WiFi.h>
#include <HTTPClient.h>
#include "router.h"

const char* serverName = "http://192.168.1.14:8000/";

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    http.begin(serverName); // Specify the URL
    int httpCode = http.GET(); // Make the request
    
     if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Raw Response: " + payload);
      
      // Split the payload by newline character
      int startIndex = 0;
      int endIndex = payload.indexOf('\n');
      while (endIndex >= 0) {
        String show = payload.substring(startIndex, endIndex);
        Serial.println("Show: " + show);
        
        startIndex = endIndex + 1;
        endIndex = payload.indexOf('\n', startIndex);
      }
      // Handle the last show (if there is no trailing newline)
      if (startIndex < payload.length()) {
        String show = payload.substring(startIndex);
        Serial.println("Show: " + show);
      }
    } else {
      Serial.println("Error on HTTP request");
    }
    
    http.end(); // Free resources
    
    delay(10000); // Wait for 10 seconds before making the next request
  } else {
    Serial.println("WiFi not connected");
  }
}
