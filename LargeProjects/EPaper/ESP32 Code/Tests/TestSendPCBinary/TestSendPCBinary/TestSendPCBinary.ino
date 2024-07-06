#include "Functions.h"
extern const char* ssid;
extern const char* password;




const int BUFFER_SIZE = 10;
char buffer[BUFFER_SIZE];
void listenForCommand() {
  if (Serial.available() > 0) {
    int len = Serial.readBytesUntil('\n', buffer, BUFFER_SIZE - 1);
    buffer[len] = '\0';  // Null-terminate the string

    if (strcmp(buffer, "update") == 0) {
      Serial.println("Getting data");
      getNewImageData();
    } else if (strcmp(buffer, "read") == 0) {
      readFileRaw(testDataFile);
    } else if (strcmp(buffer, "refresh") == 0) {
      updateEpaperImage();
      hibernateDisplay();

    } else {
      Serial.println("Unknown command");
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" connected!");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());


  initFileSystem();


  initEpaper();


  // listDir(LittleFS, "/", 1);
  // printFile(testDataFile, "dfdffsd");
  // uint8_t buffer[6] = { 1, 5, 44, 66, 6, 25 };
  // writeFile(testDataFile, buffer, 6);
  // listDir(LittleFS, "/", 1);
  // readFile(testDataFile);
  // readFileRaw(testDataFile);
}

void loop() {
  listenForCommand();
  delay(200);
}
