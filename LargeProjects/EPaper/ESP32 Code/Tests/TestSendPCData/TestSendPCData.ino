
#include "functions.h"


bool getImageData(){
  HTTPClient http;
  if (getRequest(&http, "image")) {
    Serial.println(http.getString());
    // *showData = http.getString();
    http.end();
    return true;
  }
  http.end();
  return false;
}

void setup() {
  Serial.begin(115200);

  if (connectToRouter() /* && PC server running */) {

    //getNewImage

    //Update Picture


    // String showData;
    // if (getShowData(&showData)) {
    //   Serial.println(showData);
    //   //Save to file
    // }

    getImageData();

    WiFi.disconnect();
  }

  //Load show data and look for shows

  //Update display if show out

  //Sleep

  Serial.println("Connected to WiFi");
}



void loop() {

  delay(10000);
}
