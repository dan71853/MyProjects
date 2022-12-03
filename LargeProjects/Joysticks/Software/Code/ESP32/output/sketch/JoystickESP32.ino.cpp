#include <Arduino.h>
#line 1 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
#include <Wire.h>

/*
 * ADC chip
 */
const uint8_t ADC_S0 = 19;
const uint8_t ADC_S1 = 18;
const uint8_t ADC_S2 = 21;
const uint8_t ADC_SIGNAL_PIN = 15;

/* Mani buffer array
xAxis L, xAxis H,
yAxis L, yAxis H,
RzAxis L, RzAxis H,
Mini Y axis L, Mini Y axis H,
Mini X axis L, Mini X axis H,
Button group 1  [MJS Button, trigger half, trigger full,Rest..]
*/
uint8_t mainBuffer[32];
boolean bufferRead = true;
const uint8_t bufferEnd = 12;
const uint8_t bufferButtonStart = 10;
uint8_t hash = 0;

// Set up 2 I2C busses
TwoWire I2C_Pico_ESP = TwoWire(0);
const uint8_t I2C_Pico_ESP_SDA = 22;
const uint8_t I2C_Pico_ESP_SCL = 23;

TwoWire I2C_ESP_Micro = TwoWire(1);
const uint8_t I2C_ESP_Micro_SDA = 4;
const uint8_t I2C_ESP_Micro_SCL = 16;

// unsigned long I2C_Timeout_Timer = millis();
/*
 * Button Matrix
 */

// Number of rows and columns of the button matrix
#define BM_COL_NUM 4
#define BM_ROW_NUM 1

// Pin mapping of the button matrix
uint8_t BMColPins[BM_COL_NUM] = {13, 12, 14, 27};
uint8_t BMRowPins[BM_ROW_NUM] = {25};

// Stores the state of the all the buttons in the button matrix
// Each bit is a different button, can have up to 32 buttons
uint32_t buttonStates = 0;

const uint8_t encoderPinA = 39;
const uint8_t encoderPinB = 36;
int8_t pulses = 0;
uint16_t triggerTime = millis();

/**
 * @brief Setup function
 * Setup ADC pins
 * Setup I2C busses
 * Setup button matrix
 */
#line 62 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void setup();
#line 79 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void loop();
#line 102 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void requestEvent();
#line 111 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void calculateHash();
#line 122 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void setupADC();
#line 137 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void updateAllADCs();
#line 152 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void updateADC(uint8_t MUXSelect, uint8_t bufferStart);
#line 174 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void setupButtonMatrix();
#line 188 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void updateButtons();
#line 212 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
uint8_t readTrigger(uint8_t buttonNumber);
#line 227 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void setupEncoders();
#line 232 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
uint8_t updateEncoder(uint8_t buttonNumber);
#line 272 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void testLCDCode();
#line 62 "c:\\Users\\somme\\Documents\\Git\\MyProjects\\LargeProjects\\Joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void setup() {
    Serial.begin(115200);

    setupADC();

    I2C_Pico_ESP.begin(2, I2C_Pico_ESP_SDA, I2C_Pico_ESP_SCL, 100000);
    I2C_Pico_ESP.onRequest(requestEvent);

    I2C_ESP_Micro.begin(I2C_ESP_Micro_SDA, I2C_ESP_Micro_SCL, (uint32_t)100000);
    setupButtonMatrix();
    setupEncoders();
}

/**
 * @brief Main loop
 * Update the buffer after it gets read by the pico
 */
void loop() {
    if (bufferRead) {
        updateAllADCs();
        updateButtons();
        calculateHash();
        bufferRead = false;
    }
    delay(10);
    // if (millis() - I2C_Timeout_Timer > 2000) {
    //     Serial.println("I2C Timeout");
    //     Serial.println(millis());
    //     Serial.println(I2C_Timeout_Timer);
    //     I2C_Timeout_Timer = millis();
    //     bufferRead = true;
    // }

    // testLCDCode();       //Removed when not connected
}

/**
 * @brief Called when Pico requests data
 *  write the buffer and set the read flag
 */
void requestEvent() {
    I2C_Pico_ESP.write(mainBuffer, bufferEnd);
    I2C_Pico_ESP.write(hash);
    bufferRead = true;

    // Serial.println(mainBuffer[5] | (mainBuffer[4] << 8));
    // I2C_Timeout_Timer = millis();
}

void calculateHash() {
    hash = 0;
    for (uint8_t i = 0; i < bufferEnd; i++) {
        hash += mainBuffer[i];
    }
    // Serial.println(hash);
}
/**
 * @brief Set up the ADC multiplexer
 *  Initialise all the select pins for the MUX and set them LOW
 */
void setupADC() {
    pinMode(ADC_S0, OUTPUT);
    pinMode(ADC_S1, OUTPUT);
    pinMode(ADC_S2, OUTPUT);

    digitalWrite(ADC_S0, LOW);
    digitalWrite(ADC_S1, LOW);
    digitalWrite(ADC_S2, LOW);
}

int tempInt = 1127;
/**
 * @brief Loop over all ADC inputs and save them to the buffer
 *
 */
void updateAllADCs() {
    updateADC(0, 0);  // X axis
    updateADC(1, 2);  // Y axis
    updateADC(2, 4);  // Rz Axis
    updateADC(4, 6);  // Mini Y axis
    updateADC(5, 8);  // Mini X axis
    // tempInt++;
}

/**
 * @brief Switch the MUX to the correct channel and save the data to the buffer
 *
 * @param MUXSelect MUX channel to read from
 * @param bufferStart Where to place in the buffer
 */
void updateADC(uint8_t MUXSelect, uint8_t bufferStart) {
    // Select MUX channel
    digitalWrite(ADC_S0, MUXSelect & 0b001);
    digitalWrite(ADC_S1, MUXSelect & 0b010);
    digitalWrite(ADC_S2, MUXSelect & 0b100);
    // Take measurement
    uint16_t reading = analogRead(ADC_SIGNAL_PIN);
    // reading = tempInt;
    if (reading == 1072) {
        reading = 1071;
    } else if (reading == 1277) {
        reading = 1276;
    }
    // Split into 2 8-Bit numbers and save to main buffer
    mainBuffer[bufferStart + 1] = reading & 0xFF;
    mainBuffer[bufferStart] = reading >> 8;
}

/**
 * @brief Set up the pins for the button matrix
 *  The inputs must be set to INPUT_PULLUP
 */
void setupButtonMatrix() {
    for (uint8_t col = 0; col < BM_COL_NUM; col++) {
        pinMode(BMColPins[col], INPUT_PULLUP);
    }
    for (uint8_t row = 0; row < BM_ROW_NUM; row++) {
        pinMode(BMRowPins[row], OUTPUT);
        digitalWrite(BMRowPins[row], HIGH);
    }
}

/**
 * @brief loop over every button and check its state
 * Save each button state to buttonStates
 */
void updateButtons() {
    uint8_t buttonNumber = 0;
    bool buttonState = !digitalRead(0);  // Read the mini joystick button
    buttonStates = (buttonStates & ~(1UL << buttonNumber)) | (buttonState << buttonNumber);
    buttonNumber++;
    buttonNumber = readTrigger(buttonNumber);
    for (uint8_t row = 0; row < BM_ROW_NUM; row++) {
        digitalWrite(BMRowPins[row], LOW);
        for (uint8_t col = 0; col < BM_COL_NUM; col++) {
            buttonState = !digitalRead(BMColPins[col]);
            buttonStates = (buttonStates & ~(1UL << buttonNumber)) | (buttonState << buttonNumber);
            buttonNumber++;
        }
        digitalWrite(BMRowPins[row], HIGH);
    }
    // Serial.println();
    // Serial.println(mainBuffer[bufferButtonStart], BIN);

    buttonNumber = updateEncoder(buttonNumber);
    Serial.println(buttonStates, BIN);
    mainBuffer[bufferButtonStart] = buttonStates & 0xFF;
    mainBuffer[bufferButtonStart + 1] = buttonStates >> 8 & 0xFF;
}

uint8_t readTrigger(uint8_t buttonNumber) {
    uint8_t MUXSelect = 3;
    digitalWrite(ADC_S0, MUXSelect & 0b001);
    digitalWrite(ADC_S1, MUXSelect & 0b010);
    digitalWrite(ADC_S2, MUXSelect & 0b100);
    uint16_t triggerValue = analogRead(ADC_SIGNAL_PIN);
    bool buttonState = triggerValue < 3000;
    buttonStates = (buttonStates & ~(1UL << buttonNumber)) | (buttonState << buttonNumber);
    buttonNumber++;
    buttonState = triggerValue < 800;
    buttonStates = (buttonStates & ~(1UL << buttonNumber)) | (buttonState << buttonNumber);
    buttonNumber++;
    return buttonNumber;
}

void setupEncoders() {
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);
}

uint8_t updateEncoder(uint8_t buttonNumber) {
    if (millis() - triggerTime > 200) {
        if (!digitalRead(encoderPinA)) {
            if (digitalRead(encoderPinB)) {
                pulses--;
            } else {
                pulses++;
            }
            triggerTime = millis();
        }

        bool encoderUp = 0;
        bool encoderDown = 0;
        if (pulses > 0) {
            encoderUp = 1;
            Serial.println("UP");
            pulses = 0;
        } else if (pulses < 0) {
            encoderDown = 1;
            Serial.println("Down");
            pulses = 0;
        }
        buttonStates = (buttonStates & ~(1UL << buttonNumber)) | (encoderUp << buttonNumber);
        buttonNumber++;
        buttonStates = (buttonStates & ~(1UL << buttonNumber)) | (encoderDown << buttonNumber);
        buttonNumber++;
        return buttonNumber;
    } else {
        return buttonNumber + 2;
    }
}

/**
 * @brief Lest lcd and led sending
 * Send [uint16_t LCD digit, int8_t LCD symbol state, int8_t LED digit 0,  int8_t LED digit 1]
 */
// Temp  variables to control LCD
unsigned long tempTimer = millis();
uint16_t tempCounter = 0;
int8_t SymbolState = 0;
void testLCDCode() {
    if (millis() - tempTimer > 1000) {
        int number = random(0, 99);
        if (SymbolState == 0) {
            SymbolState = 1;
        } else {
            SymbolState = 0;
        }
        // Serial.println(number);
        I2C_ESP_Micro.beginTransmission(3);
        I2C_ESP_Micro.write(number);
        I2C_ESP_Micro.write(SymbolState);

        I2C_ESP_Micro.write(tempCounter & 0xFF);
        I2C_ESP_Micro.write((tempCounter & 0xFF00) >> 8);
        tempCounter++;
        I2C_ESP_Micro.endTransmission();

        tempTimer = millis();
    }
}

