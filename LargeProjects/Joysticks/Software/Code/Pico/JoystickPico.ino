#include <PicoGamepad1.h>
// #include <PicoGamepad2.h>
#include <Wire.h>

uint8_t mainBuffer[32];
const uint8_t bufferEnd = 12;
const uint8_t bufferButtonStart = 10;

PicoGamepad1 gamepad1;
// PicoGamepad2 gamepad2;

struct {
    int x;
    int y;
    int Rx;
    int Ry;
    int Rz;
} axisData;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Serial.print(analogRead(A0));  // Y
    // Serial.print(',');
    // Serial.print(analogRead(A1));  // X
    // Serial.print(',');
    // Serial.print(analogRead(A2));  // Rot
    // Serial.println();
}

void loop() {
    requestData();
    readAllAxis();
    readButtons();
    setAllAxis();
    // Serial.print(axisData.x);
    // Serial.print(',');
    // Serial.print(axisData.y);
    // Serial.print(',');
    Serial.print(axisData.Rz);  //

    Serial.println();
    if (calculateHash() == mainBuffer[bufferEnd]) {
        unsigned long tempTimer = millis();
        gamepad1.send_update();
        // gamepad2.send_update();
        while (millis() - tempTimer < 10) {
        }
    } else {
        Serial.println("Hash error");
        delay(100);
    }
}

uint8_t calculateHash() {
    uint8_t hash = 0;
    for (uint8_t i = 0; i < bufferEnd; i++) {
        hash += mainBuffer[i];
    }
    // Serial.println(hash);
    return hash;
}

void requestData() {
    // Serial.println("Sending Request");
    Wire.requestFrom(2, bufferEnd + 1);
    // Serial.println("Request Sent");
    uint8_t index = 0;
    while (Wire.available()) {
        // Serial.print(".");
        uint8_t num = Wire.read();
        // Serial.println(num, HEX); 882_1000
        mainBuffer[index++] = num;
    }
    // Serial.println();
    // Serial.println(mainBuffer[5] | (mainBuffer[4] << 8));
    // Serial.println("Data received");
}

void readAllAxis() {
    axisData.x = mainBuffer[1] | mainBuffer[0] << 8;
    axisData.y = mainBuffer[3] | mainBuffer[2] << 8;
    axisData.Rz = mainBuffer[5] | mainBuffer[4] << 8;
    axisData.Ry = mainBuffer[7] | mainBuffer[6] << 8;
    axisData.Rx = mainBuffer[9] | mainBuffer[8] << 8;
}

void readButtons() {
    // Serial.println();
    // Serial.println(mainBuffer[10], BIN);
    // Serial.println(mainBuffer[11]);
    for (uint8_t buttonNum = 0; buttonNum < 8; buttonNum++) {
        gamepad1.SetButton(buttonNum, (mainBuffer[bufferButtonStart] >> buttonNum) & 1);
        gamepad1.SetButton(buttonNum + 8, (mainBuffer[bufferButtonStart + 1] >> buttonNum) & 1);
    }
}
void setAllAxis() {
    gamepad1.SetX(axisData.x);
    gamepad1.SetY(axisData.y);
    gamepad1.SetZ(axisData.Rz);

    gamepad1.SetRx(analogRead(A0));
    gamepad1.SetRy(analogRead(A1));
    gamepad1.SetRz(analogRead(A2));
}
