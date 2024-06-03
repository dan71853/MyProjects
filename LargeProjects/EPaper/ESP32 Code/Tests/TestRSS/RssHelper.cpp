#include "functions.h"


static rssRead rss;
const char* search_keywords[] = {
  "[EMBER]",
  "[Xtrem]",
  "[DKB]",
  "[SubsPlease]",
  "[LostYears]"
};


bool isUploaderTrusted(String* title) {
  for (const char* keyword : search_keywords) {
    if (title->indexOf(keyword) >= 0) {
      return true;
    }
  }
  return false;
}

void isShowOut(const char* showUrl) {

  rss.begin();   //init the rss class
  rss.axs(showUrl);  //Request the data
  //Wait for response

  while (1) {
    String dst = rss.finds(String("title"));  //Get only the title
    if (!dst.length()) break;

    if (isUploaderTrusted(&dst)) {
      Serial.println(dst);
      rss.end();
      return;
    }

    // Serial.printf("[%s]\n", dst.c_str());
  }
  rss.end();
}

