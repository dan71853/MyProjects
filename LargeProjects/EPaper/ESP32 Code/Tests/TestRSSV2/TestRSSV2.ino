#include <WiFi.h>
#include <HTTPClient.h>
#include "functions.h"


const char* url = "https://nyaa.si/?page=rss&q=Kaiju+s01e01+1080p&c=0_0&f=0";
const String _xml = "<rss xmlns:atom=http://www.w3.org/2005/Atom xmlns:nyaa=https://nyaa.si/xmlns/nyaa version=2.0>\n	<channel>\n		<title>Nyaa - &34;Kaiju s01e01 1080p&34; - Torrent File RSS</title>\n		<description>RSS Feed for &34;Kaiju s01e01 1080p&34;</description>\n		<link>https://nyaa.si/</link>\n		<atom:link href=https://nyaa.si/?page=rss rel=self type=application/rss+xml />\n		<item>\n			<title>[EMBER] Kaijuu 8-gou S01E01-06 [Dual Audio] (1080p AV1 AAC) [Multi Subs] (EN|AR|DE|ES-LA|ES|FR|IT|PT-BR|RU) | Kaiju No. 8 | Monster 8 (Batch)</title>\n				<link>https://nyaa.si/download/1821104.torrent</link>\n				<guid isPermaLink=true>https://nyaa.si/view/1821104</guid>\n				<pubDate>Sat, 18 May 2024 22:52:09 -0000</pubDate>\n				<nyaa:seeders>16</nyaa:seeders>\n				<nyaa:leechers>0</nyaa:leechers>\n				<nyaa:downloads>469</nyaa:downloads>\n				<nyaa:infoHash>7b16757ee3e9a3d69ce57b4a03299380cef8625b</nyaa:infoHash>\n			<nyaa:categoryId>1_2</nyaa:categoryId>\n			<nyaa:category>Anime - English-translated</nyaa:category>\n			<nyaa:size>2.0 GiB</nyaa:size>\n			<nyaa:comments>0</nyaa:comments>\n			<nyaa:trusted>No</nyaa:trusted>\n			<nyaa:remake>Yes</nyaa:remake>\n			<description><![CDATA[<a href=https://nyaa.si/view/1821104>1821104 | [Trix] Kaijuu 8-gou S01E01-06 [Dual Audio] (1080p AV1 AAC) [Multi Subs] (EN|AR|DE|ES-LA|ES|FR|IT|PT-BR|RU) | Kaiju No. 8 | Monster 8 (Batch)</a> | 2.0 GiB | Anime - English-translated | 7b16757ee3e9a3d69ce57b4a03299380cef8625b]]></description>\n		</item>";


void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  delay(1000);

  String outCollection;
  readXML(_xml, &outCollection);
  readXML(_xml, &outCollection);
  readXML(_xml, &outCollection);
  Serial.println(outCollection);

  return;


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



  if (false && WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(payload);
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error in Wi-Fi connection");
  }



  // isShowOut(url);

  //  rss.dumpXml();

  WiFi.disconnect();

  // ESP.deepSleep(6e7);  //9e8 = 15Min
}

void loop() {
  // put your main code here, to run repeatedly:
}
