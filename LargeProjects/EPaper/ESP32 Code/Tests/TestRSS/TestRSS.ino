#include <WiFi.h>
#include "rssRead.hpp"
#include "router.h"

const char* ssid = "Toaster 2.4GHz";
const char* password = "wildhunt";

const char* url = "https://nyaa.si/?page=rss&q=Kaiju+ember+s01e01+1080p&c=0_0&f=0";

static rssRead rss;

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
  rss.begin();
  rss.axs(url);
   rss.dumpXml();
  while (1) {
    String dst = rss.finds(String("title"));
    if (!dst.length()) break;
    Serial.printf("[%s]\n", dst.c_str());
  }
  Serial.printf("<== End rssRead(%d)\n", rss.tagCnt());
  rss.end();
  WiFi.disconnect();
}

void loop() {
  // put your main code here, to run repeatedly:
}
