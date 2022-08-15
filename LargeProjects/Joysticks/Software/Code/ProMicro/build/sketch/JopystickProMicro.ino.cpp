#include <Arduino.h>
#line 1 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
#include <Wire.h>

#define SPILOAD 14
#define SPICLK 15
#define SPIMOSI 16

uint8_t numberOfLEDDigits = 2;
uint8_t LEDStates[2] = {0, 0};

int LCDDigits = 0;     // int number 00 - 99
uint8_t LCDOther = 0;  // [0, 0, 0, 0, ALL, PROG, REPEAT, ARROW]
int comState = 0;
int comStates[8] = {2, 1, 1, 1, 0, 1, 1, 1};
int pinMap[4] = {0, 1, 4, 5};

int32_t rightDigits[16] = {0xFA000, 0x60000, 0xBC000, 0xF4000, 0x66000, 0xD6000, 0xDE000, 0x70000, 0xFE000, 0xF6000};
int32_t leftDigits[16] = {0xFA0, 0x600, 0xBC0, 0xF40, 0x660, 0xD60, 0xDE0, 0x700, 0xFE0, 0xF60};

#line 19 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void setup();
#line 32 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void loop();
#line 36 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void COMController();
#line 60 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
uint32_t calculateHexCode();
#line 76 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
int getComBit(int index);
#line 81 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void setPin(int pinIndex, int pinState);
#line 105 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void receiveEvent(int howMany);
#line 120 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void setupLEDDriver(uint8_t LEDNum);
#line 143 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void writeRegisterSPI(uint8_t address, uint8_t data);
#line 166 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void updateLEDs();
#line 19 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void setup() {
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);

    Wire.begin(3);
    Wire.onReceive(receiveEvent);

    setupLEDDriver(numberOfLEDDigits);
}

void loop() {
    COMController();
}

void COMController() {
    uint32_t selectBit = calculateHexCode();

    for (int COM = 0; COM < 4; COM++) {
        setPin(COM, getComBit(comState - COM));
    }
    for (int selectPin = 0; selectPin < 5; selectPin++) {
        if (comState < 4) {
            digitalWrite(selectPin + 6, !bitRead(selectBit, selectPin * 4 + comState));
        } else {
            digitalWrite(selectPin + 6, bitRead(selectBit, selectPin * 4 + comState - 4));
        }
    }
    comState++;

    if (comState >= 8) {
        comState = 0;
    }
    if (comState < 0) {
        comState = 7;
    }
    delay(5);
}

uint32_t calculateHexCode() {
    uint32_t output = leftDigits[LCDDigits / 10] | rightDigits[LCDDigits % 10];
    if (LCDOther & 0b1) {
        output |= 0x1000;
    }
    if (LCDOther & 0b10) {
        output |= 0x4;
    }
    if (LCDOther & 0b100) {
        output |= 0x8;
    }
    if (LCDOther & 0b1000) {
        output |= 0x10;
    }
    return output;
}
int getComBit(int index) {
    index = (8 + (index % 8)) % 8;
    return comStates[index];
}

void setPin(int pinIndex, int pinState) {
    int pin = pinMap[pinIndex];
    switch (pinState) {
        case 0:
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);
            break;
        case 1:
            pinMode(pin, INPUT);
            break;
        case 2:
            pinMode(pin, OUTPUT);
            digitalWrite(pin, HIGH);
            break;
        default:
            break;
    }
}

/**
 * @brief Method called when I2C receive message received
 *
 * @param howMany
 */
void receiveEvent(int howMany) {
    LCDDigits = Wire.read();
    LCDOther = Wire.read();

    LEDStates[0] = (uint8_t)Wire.read();
    LEDStates[1] = (uint8_t)Wire.read();

    updateLEDs();
}

/**
 * @brief Set up the MAX7219 LED driver
 *
 * @param LEDNum
 */
void setupLEDDriver(uint8_t LEDNum) {
    pinMode(SPICLK, OUTPUT);
    pinMode(SPILOAD, OUTPUT);
    pinMode(SPIMOSI, OUTPUT);

    digitalWrite(SPILOAD, HIGH);  // Set the load bit HIGH before any data is sent

    writeRegisterSPI(0x0C, 0x01);  // Shutdown Register, set to normal operation
    writeRegisterSPI(0x0F, 0x00);  // Display-Test Register, set to normal operation

    writeRegisterSPI(0x09, 0x00);        // Decode-Mode Register, set to no decode
    writeRegisterSPI(0x0B, LEDNum - 1);  // Scan-Limit Register, set to LEDNum digits

    writeRegisterSPI(0x01, 0x00);  // Turn off all segments of digit 1
    writeRegisterSPI(0x02, 0x00);  // Turn off all segments of digit 2
}

/**
 * @brief BitBang SPI method
 *
 * @param address Address of regester to write to
 * @param data Data to send
 */
void writeRegisterSPI(uint8_t address, uint8_t data) {
    digitalWrite(SPILOAD, LOW);  // Set load LOW, indicates start of message

    // Send addtress byte first
    for (uint8_t i = 0; i < 8; i++) {                       // Loop over each byte in reverse order
        digitalWrite(SPIMOSI, address & (0x1 << (7 - i)));  // Set the MOSI pin to the current bit
        digitalWrite(SPICLK, HIGH);                         // Pulse the clock
        digitalWrite(SPICLK, LOW);
    }
    // Send data byte
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(SPIMOSI, data & (0x1 << (7 - i)));
        digitalWrite(SPICLK, HIGH);
        digitalWrite(SPICLK, LOW);
    }

    digitalWrite(SPILOAD, HIGH);  // Set load HIGH, indicates end of message
}

/**
 * @brief Update all the LED registers
 *
 */
void updateLEDs() {
    for (uint8_t i = 0; i < numberOfLEDDigits; i++) {  // Loop over all the digits
        writeRegisterSPI(i + 1, LEDStates[i]);
    }
}

