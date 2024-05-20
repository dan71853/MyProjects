
#include "functions.h"




void setup() {
  Serial.begin(115200);

  if (connectToRouter()) {

    String showData;
    if (getShowData(&showData)) {
      Serial.println(showData);
      //Save to file
    }
  }


  Serial.println("Connected to WiFi");
}



void loop() {

  delay(10000);
}
