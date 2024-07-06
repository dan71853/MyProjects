#include <WiFi.h>
#include "Functions.h"
extern const char* ssid;
extern const char* password;
const char* server_ip = "192.168.1.14";
const uint16_t server_port = 8001;

WiFiClient client;
void getData() {
  // Connect to server

  File file = LittleFS.open(testDataFile, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }

  if (client.connect(server_ip, server_port)) {
    Serial.println("Connected to server");
    uint32_t counter = 0;
    uint32_t timer = millis();
    // Read the binary data
    while (client.connected() || client.available()) {
      if (client.available()) {

        uint8_t data = client.read();
        counter++;
        // Serial.printf("byte %d: %02X\n", counter++, data);
        if (file.write(data)) {
          // Serial.println("- file written");
        } else {
          Serial.println("- write failed");
        }
      }
    }
    Serial.printf("Time: %d, count:%d\n", millis() - timer, counter);
    file.close();
    client.stop();
    Serial.println("Disconnected from server");
  } else {
    Serial.println("Connection to server failed");
  }
}

const int BUFFER_SIZE = 10;
char buffer[BUFFER_SIZE];
void listenForCommand() {
  if (Serial.available() > 0) {
    int len = Serial.readBytesUntil('\n', buffer, BUFFER_SIZE - 1);
    buffer[len] = '\0';  // Null-terminate the string

    if (strcmp(buffer, "update") == 0) {
      Serial.println("Getting data");
      getData();
    } else if (strcmp(buffer, "read") == 0) {
      readFileRaw(testDataFile);
    } else if (strcmp(buffer, "refresh") == 0) {
      drawBitmaps7c300x180();
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
