#include "../Inc/TimeServer.h"
void getTimeFromNTP() {
    const char *TS_ssid = "Stuff-Fibre_68_2G";
    const char *TS_password = "06115593";

    // const char *TS_ssid = "edimax_2.4G_19403C";
    // const char *TS_password = "VMW9X9KC";

    const char *TS_ntpServer = "pool.ntp.org";
    const long TS_gmtOffset_sec = 0;
    const int TS_daylightOffset_sec = 0;

    Serial.printf("Connecting to %s ", TS_ssid);

    WiFi.begin(TS_ssid, TS_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");
    configTime(TS_gmtOffset_sec, TS_daylightOffset_sec, TS_ntpServer);
    // getLocalTimeString();
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        configTime(TS_gmtOffset_sec, TS_daylightOffset_sec, TS_ntpServer);
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Failed to obtain time");
        }
    }

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void getLocalTimeString(char *buffer) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

    sprintf(buffer, "%02d_%02d_%02d %02d_%02d_%02d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year - 100, (timeinfo.tm_hour + 13) % 24, timeinfo.tm_min, timeinfo.tm_sec);
}