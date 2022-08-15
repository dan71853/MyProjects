#include <Arduino.h>
#line 1 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\ESPCamera\\TimeLapse\\ESPTimelapse.ino"
#include <WiFi.h>

#include "Inc/Camera.h"
#include "Inc/TimeServer.h"
#include "time.h"

RTC_DATA_ATTR int bootCount = 0;

#line 9 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\ESPCamera\\TimeLapse\\ESPTimelapse.ino"
void setup();
#line 62 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\ESPCamera\\TimeLapse\\ESPTimelapse.ino"
void loop();
#line 9 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\ESPCamera\\TimeLapse\\ESPTimelapse.ino"
void setup() {
    Serial.begin(115200);
    setupCamera();
    setupSD();

    if (bootCount == 0) {
        Serial.print("Starting");
        getTimeFromNTP();
    }

    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
    delay(3000);
    char buffer[32];
    getLocalTimeString(buffer);
    String path = "/TimeLapse/" + String(buffer) + ".jpg";
    Serial.println(path);

    camera_fb_t *fb = NULL;

    // Take Picture with Camera
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    fs::FS &fs = SD_MMC;
    Serial.printf("Picture file name: %s\n", path.c_str());

    File file = fs.open(path.c_str(), FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file in writing mode");
    } else {
        file.write(fb->buf, fb->len); // payload (image), payload length
        Serial.printf("Saved file to path: %s\n", path.c_str());
    }
    file.close();
    esp_camera_fb_return(fb);

    delay(5000);

    esp_sleep_enable_timer_wakeup(20 * 60 * 1000000);
    // esp_sleep_enable_timer_wakeup(10 * 1000000);
    Serial.println("Setup ESP32 to sleep for every " + String(5) + " Min");

    Serial.println("Going to sleep now");
    delay(1000);
    Serial.flush();
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}

void loop() {
}

