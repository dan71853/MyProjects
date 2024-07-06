#include "Functions.h"

const char* server_ip = "192.168.1.14";
const uint16_t server_port = 8001;

WiFiClient client;
void getNewImageData() {
  // Connect to server

  File file = LittleFS.open(imageDataFile, FILE_WRITE);
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


