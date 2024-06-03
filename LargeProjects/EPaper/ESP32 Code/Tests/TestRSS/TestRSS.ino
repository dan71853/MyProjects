#include <WiFi.h>
#include "functions.h"
#include "router.h"


const char* url = "https://nyaa.si/?page=rss&q=Kaiju+s01e01+1080p&c=0_0&f=0";


void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  Serial.println("Start rssRead ==>");

  isShowOut(url);

  //  rss.dumpXml();

  WiFi.disconnect();

  ESP.deepSleep(6e7);  //9e8 = 15Min
}

void loop() {
  // put your main code here, to run repeatedly:
}
