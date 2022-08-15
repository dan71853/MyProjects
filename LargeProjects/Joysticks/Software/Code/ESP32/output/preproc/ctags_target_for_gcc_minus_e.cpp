# 1 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
# 2 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino" 2

/*

 * ADC chip

 */
# 6 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
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
# 19 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
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
# 39 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
// Number of rows and columns of the button matrix



// Pin mapping of the button matrix
uint8_t BMColPins[4] = {13, 12, 14, 27};
uint8_t BMRowPins[1] = {25};

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
# 62 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
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
# 79 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
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
# 102 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
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
# 122 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void setupADC() {
    pinMode(ADC_S0, 0x02);
    pinMode(ADC_S1, 0x02);
    pinMode(ADC_S2, 0x02);

    digitalWrite(ADC_S0, 0x0);
    digitalWrite(ADC_S1, 0x0);
    digitalWrite(ADC_S2, 0x0);
}

int tempInt = 1127;
/**

 * @brief Loop over all ADC inputs and save them to the buffer

 *

 */
# 137 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void updateAllADCs() {
    updateADC(0, 0); // X axis
    updateADC(1, 2); // Y axis
    updateADC(2, 4); // Rz Axis
    updateADC(4, 6); // Mini Y axis
    updateADC(5, 8); // Mini X axis
    // tempInt++;
}

/**

 * @brief Switch the MUX to the correct channel and save the data to the buffer

 *

 * @param MUXSelect MUX channel to read from

 * @param bufferStart Where to place in the buffer

 */
# 152 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
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
# 174 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void setupButtonMatrix() {
    for (uint8_t col = 0; col < 4; col++) {
        pinMode(BMColPins[col], 0x05);
    }
    for (uint8_t row = 0; row < 1; row++) {
        pinMode(BMRowPins[row], 0x02);
        digitalWrite(BMRowPins[row], 0x1);
    }
}

/**

 * @brief loop over every button and check its state

 * Save each button state to buttonStates

 */
# 188 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
void updateButtons() {
    uint8_t buttonNumber = 0;
    bool buttonState = !digitalRead(0); // Read the mini joystick button
    buttonStates = (buttonStates & ~(1UL << buttonNumber)) | (buttonState << buttonNumber);
    buttonNumber++;
    buttonNumber = readTrigger(buttonNumber);
    for (uint8_t row = 0; row < 1; row++) {
        digitalWrite(BMRowPins[row], 0x0);
        for (uint8_t col = 0; col < 4; col++) {
            buttonState = !digitalRead(BMColPins[col]);
            buttonStates = (buttonStates & ~(1UL << buttonNumber)) | (buttonState << buttonNumber);
            buttonNumber++;
        }
        digitalWrite(BMRowPins[row], 0x1);
    }
    // Serial.println();
    // Serial.println(mainBuffer[bufferButtonStart], BIN);

    buttonNumber = updateEncoder(buttonNumber);
    Serial.println(buttonStates, 2);
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
    pinMode(encoderPinA, 0x01);
    pinMode(encoderPinB, 0x01);
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
# 268 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\joysticks\\Software\\Code\\ESP32\\JoystickESP32.ino"
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
