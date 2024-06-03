#include "functions.h"


const char* search_keywords[] = {
  "[EMBER]",
  "[Xtrem]",
  "[DKB]",
  "[SubsPlease]",
  "[LostYears]"
};


bool isUploaderTrusted(String title) {
  for (const char* keyword : search_keywords) {
    if (title.indexOf(keyword) >= 0) {
      return true;
    }
  }
  return false;
}


void readXML(String xml, String* outCollection) {

  // Serial.println(xml);
  uint16_t startIndex = 0;
  uint16_t xmlLength = xml.length();

  while (startIndex < xmlLength) {
    int16_t tagStart = xml.indexOf("<title>", startIndex);
    if (tagStart == -1) {
      Serial.println("Start Not found");
      return;
    }
    int16_t tagEnd = xml.indexOf("</title>", tagStart);
    if (tagEnd == -1) {
      Serial.println("End Not found");
      return;
    }
    startIndex = tagEnd;
    String titleText = xml.substring(tagStart + 7, tagEnd);
    if (isUploaderTrusted(titleText)) {
      *outCollection = *outCollection + titleText + '\n';
      return;
    }
  }
}
