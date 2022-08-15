# 1 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
# 2 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino" 2





uint8_t numberOfLEDDigits = 2;
uint8_t LEDStates[2] = {0, 0};

int LCDDigits = 0; // int number 00 - 99
uint8_t LCDOther = 0; // [0, 0, 0, 0, ALL, PROG, REPEAT, ARROW]
int comState = 0;
int comStates[8] = {2, 1, 1, 1, 0, 1, 1, 1};
int pinMap[4] = {0, 1, 4, 5};

int32_t rightDigits[16] = {0xFA000, 0x60000, 0xBC000, 0xF4000, 0x66000, 0xD6000, 0xDE000, 0x70000, 0xFE000, 0xF6000};
int32_t leftDigits[16] = {0xFA0, 0x600, 0xBC0, 0xF40, 0x660, 0xD60, 0xDE0, 0x700, 0xFE0, 0xF60};

void setup() {
    pinMode(6, 0x1);
    pinMode(7, 0x1);
    pinMode(8, 0x1);
    pinMode(9, 0x1);
    pinMode(10, 0x1);

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
            digitalWrite(selectPin + 6, !(((selectBit) >> (selectPin * 4 + comState)) & 0x01));
        } else {
            digitalWrite(selectPin + 6, (((selectBit) >> (selectPin * 4 + comState - 4)) & 0x01));
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
            pinMode(pin, 0x1);
            digitalWrite(pin, 0x0);
            break;
        case 1:
            pinMode(pin, 0x0);
            break;
        case 2:
            pinMode(pin, 0x1);
            digitalWrite(pin, 0x1);
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
# 105 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
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
# 120 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void setupLEDDriver(uint8_t LEDNum) {
    pinMode(15, 0x1);
    pinMode(14, 0x1);
    pinMode(16, 0x1);

    digitalWrite(14, 0x1); // Set the load bit HIGH before any data is sent

    writeRegisterSPI(0x0C, 0x01); // Shutdown Register, set to normal operation
    writeRegisterSPI(0x0F, 0x00); // Display-Test Register, set to normal operation

    writeRegisterSPI(0x09, 0x00); // Decode-Mode Register, set to no decode
    writeRegisterSPI(0x0B, LEDNum - 1); // Scan-Limit Register, set to LEDNum digits

    writeRegisterSPI(0x01, 0x00); // Turn off all segments of digit 1
    writeRegisterSPI(0x02, 0x00); // Turn off all segments of digit 2
}

/**

 * @brief BitBang SPI method

 *

 * @param address Address of regester to write to

 * @param data Data to send

 */
# 143 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void writeRegisterSPI(uint8_t address, uint8_t data) {
    digitalWrite(14, 0x0); // Set load LOW, indicates start of message

    // Send addtress byte first
    for (uint8_t i = 0; i < 8; i++) { // Loop over each byte in reverse order
        digitalWrite(16, address & (0x1 << (7 - i))); // Set the MOSI pin to the current bit
        digitalWrite(15, 0x1); // Pulse the clock
        digitalWrite(15, 0x0);
    }
    // Send data byte
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(16, data & (0x1 << (7 - i)));
        digitalWrite(15, 0x1);
        digitalWrite(15, 0x0);
    }

    digitalWrite(14, 0x1); // Set load HIGH, indicates end of message
}

/**

 * @brief Update all the LED registers

 *

 */
# 166 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ProMicro\\JopystickProMicro.ino"
void updateLEDs() {
    for (uint8_t i = 0; i < numberOfLEDDigits; i++) { // Loop over all the digits
        writeRegisterSPI(i + 1, LEDStates[i]);
    }
}
