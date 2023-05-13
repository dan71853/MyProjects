// #include <analogWrite.h>

// https://esphome.io/devices/nodemcu_esp32.html

const uint8_t PWM_CH_R = 0;
const uint8_t PWM_CH_G = 1;
const uint8_t PWM_CH_B = 2;
const uint8_t PWM_CH_R_HEAD = 3;
const uint8_t PWM_CH_G_HEAD = 4;
const uint8_t PWM_CH_B_HEAD = 5;


#define BB_PINS
#if defined BB_PINS
const uint8_t LED_R = 14;
const uint8_t LED_G = 13;
const uint8_t LED_B = 5;
const uint8_t LED_R_HEAD = 27;
const uint8_t LED_G_HEAD = 26;
const uint8_t LED_B_HEAD = 25;
const uint8_t headSwitch = 33;  // Head- latching
const uint8_t button = 0;       // Momentary
const uint8_t potSwitch = 35;   // Potswitch
const uint8_t potValue = 34;
#elif defined PCB_PINS
const uint8_t LED_R = 17;
const uint8_t LED_G = 16;
const uint8_t LED_B = 18;
const uint8_t LED_R_HEAD = 27;
const uint8_t LED_G_HEAD = 26;
const uint8_t LED_B_HEAD = 25;
const uint8_t headSwitch = 33;  // Head- latching
const uint8_t button = 19;      // Momentary
const uint8_t potSwitch = 35;   // Potswitch
const uint8_t potValue = 32;
#endif

enum states { LIGHTS_ON,
              LIGHTS_OFF,
              M_MODE_1,
              M_MODE_2,
              M_MODE_3 };

states currentState = LIGHTS_ON;
bool stateChange = true;
unsigned long serialTimeoutTimer = millis();


const int freq = 8000;
const int resolution = 8;
const uint16_t potThreshold = 1000;
// delay after state change;
bool headMatchMain = false;
volatile bool buttonPressed = false;

uint8_t red, green, blue;  // Colour values


volatile unsigned long debouncingTimer = millis();
// TODO: remove

void IRAM_ATTR buttonInterrupt() {

    // noInterrupts();
    if (millis() - debouncingTimer > 500) {
        //     Serial.println("INT");
        buttonPressed = true;
        debouncingTimer = millis();
    }
    // interrupts();
}

void setup() {
    Serial.begin(9600);

    analogWriteFrequency(freq);  // Set the PWM frequency
    analogReadResolution(10);
    // Setup main LED PWM
    ledcSetup(PWM_CH_R, freq, resolution);
    ledcAttachPin(LED_R, PWM_CH_R);
    ledcSetup(PWM_CH_G, freq, resolution);
    ledcAttachPin(LED_G, PWM_CH_G);
    ledcSetup(PWM_CH_B, freq, resolution);
    ledcAttachPin(LED_B, PWM_CH_B);

    // Setup head LED PWM
    ledcSetup(PWM_CH_R_HEAD, freq, resolution);
    ledcAttachPin(LED_R_HEAD, PWM_CH_R_HEAD);
    ledcSetup(PWM_CH_G_HEAD, freq, resolution);
    ledcAttachPin(LED_G_HEAD, PWM_CH_G_HEAD);
    ledcSetup(PWM_CH_B_HEAD, freq, resolution);
    ledcAttachPin(LED_B_HEAD, PWM_CH_B_HEAD);

    // Setup buttons and enable interrupt for momentary button
    pinMode(headSwitch, INPUT_PULLUP);
    pinMode(button, INPUT_PULLUP);
    attachInterrupt(button, buttonInterrupt, FALLING);
    pinMode(potSwitch, INPUT_PULLUP);

    // Turn all lights off
    ledcWrite(PWM_CH_R, 0);
    ledcWrite(PWM_CH_G, 0);
    ledcWrite(PWM_CH_B, 0);
    ledcWrite(PWM_CH_R_HEAD, 0x50 * 0.6);
    ledcWrite(PWM_CH_G_HEAD, 0x80 * 0.6);
    ledcWrite(PWM_CH_B_HEAD, 0x00 * 0.6);
}

void loop() {
    Serial.println(currentState);
    switch (currentState) {
        case LIGHTS_ON:  //0
            {
                if (stateChange) {
                    stateChange = false;
                }
                if (!digitalRead(headSwitch)) {
                    currentState = LIGHTS_OFF;
                    stateChange = true;
                }

                if (analogRead(potValue) < potThreshold) {
                    currentState = M_MODE_1;
                    stateChange = true;
                }

                if (buttonPressed) {
                    headMatchMain = !headMatchMain;  // Toggle the head lights state
                    buttonPressed = false;
                    if (!headMatchMain) {  // Set to default orange colour
                        ledcWrite(PWM_CH_R_HEAD, 0x50 * 0.6);
                        ledcWrite(PWM_CH_G_HEAD, 0x80 * 0.6);
                        ledcWrite(PWM_CH_B_HEAD, 0x00 * 0.6);
                    }
                }



                if (Serial.available() >= 4) {      // If there is data in the serial buffer then read
                    serialTimeoutTimer = millis();  // Reset the timeout timer
                    if (Serial.read() == 0xff) {    // Start of message
                        red = (Serial.read());      // Read colour values
                        green = (Serial.read());
                        blue = (Serial.read());
                        // digitalWrite(INBUILT_LED, uint8_t val)
                    }
                }


                if (millis() - serialTimeoutTimer > 5000) {  // If no serial data for 10 seconds then turn off lights
                    red = 0;
                    green = 0;
                    blue = 0;
                    ledcWrite(PWM_CH_R, red);
                    ledcWrite(PWM_CH_G, green * .988);
                    ledcWrite(PWM_CH_B, blue * 0.98);
                    delay(1000);
                }
                // red = 255;
                // green = 255;
                // blue = 255;
                // delay(10);

                ledcWrite(PWM_CH_R, red);
                ledcWrite(PWM_CH_G, green * .988);
                ledcWrite(PWM_CH_B, blue * 0.98);

                if (headMatchMain) {
                    ledcWrite(PWM_CH_R_HEAD, red * .9);
                    ledcWrite(PWM_CH_G_HEAD, green);
                    ledcWrite(PWM_CH_B_HEAD, blue);
                }
                break;
            }
        case LIGHTS_OFF:  //1
            {             // Could add sleep?
                if (stateChange) {
                    ledcWrite(PWM_CH_R, 0x00);
                    ledcWrite(PWM_CH_G, 0x00);
                    ledcWrite(PWM_CH_B, 0x00);
                    ledcWrite(PWM_CH_R_HEAD, 0x00);
                    ledcWrite(PWM_CH_G_HEAD, 0x00);
                    ledcWrite(PWM_CH_B_HEAD, 0x00);
                    stateChange = false;
                }
                if (digitalRead(headSwitch)) {
                    currentState = LIGHTS_ON;
                    stateChange = true;
                }
                if (buttonPressed) {
                    buttonPressed = false;
                }
                delay(1000);

                break;
            }
        case M_MODE_1:  //2
            {
                if (stateChange) {
                    stateChange = false;
                }
                if (!digitalRead(headSwitch)) {
                    currentState = LIGHTS_OFF;
                    stateChange = true;
                }
                if (analogRead(potValue) > potThreshold) {
                    currentState = LIGHTS_ON;
                    stateChange = true;
                }
                if (buttonPressed) {
                    currentState = M_MODE_2;
                    buttonPressed = false;
                    stateChange = true;
                }
                uint8_t brightness = (1023 - analogRead(potValue)) / 4;
                ledcWrite(PWM_CH_R, brightness);
                ledcWrite(PWM_CH_G, brightness * 0.988);
                ledcWrite(PWM_CH_B, brightness * 0.98);
                if (headMatchMain) {
                    ledcWrite(PWM_CH_R_HEAD, brightness);
                    ledcWrite(PWM_CH_G_HEAD, brightness);
                    ledcWrite(PWM_CH_B_HEAD, brightness);
                }

                delay(500);
                break;
            }
        case M_MODE_2:  //3
            {
                if (stateChange) {
                    ledcWrite(PWM_CH_R, 0x50);
                    ledcWrite(PWM_CH_G, 0x80);
                    ledcWrite(PWM_CH_B, 0x00);
                    if (headMatchMain) {
                        ledcWrite(PWM_CH_R_HEAD, 0x50);
                        ledcWrite(PWM_CH_G_HEAD, 0x80);
                        ledcWrite(PWM_CH_B_HEAD, 0x00);
                    }
                    stateChange = false;
                }
                if (!digitalRead(headSwitch)) {
                    currentState = LIGHTS_OFF;
                    stateChange = true;
                }
                if (analogRead(potValue) > potThreshold) {
                    currentState = LIGHTS_ON;
                    stateChange = true;
                }
                if (buttonPressed) {
                    currentState = M_MODE_3;
                    buttonPressed = false;
                    stateChange = true;
                }
                break;
            }
        case M_MODE_3:  //4
            {           // 9010dd

                if (stateChange) {
                    ledcWrite(PWM_CH_R, 0x90);
                    ledcWrite(PWM_CH_G, 0x10);
                    ledcWrite(PWM_CH_B, 0xDD);
                    if (headMatchMain) {
                        ledcWrite(PWM_CH_R_HEAD, 0x90);
                        ledcWrite(PWM_CH_G_HEAD, 0x10);
                        ledcWrite(PWM_CH_B_HEAD, 0xDD);
                    }
                    stateChange = false;
                }
                if (!digitalRead(headSwitch)) {
                    currentState = LIGHTS_OFF;
                    stateChange = true;
                }
                if (analogRead(potValue) > potThreshold) {
                    currentState = LIGHTS_ON;
                    stateChange = true;
                }
                if (buttonPressed) {
                    currentState = M_MODE_1;
                    buttonPressed = false;
                }
                break;
            }
        default:
            {
                delay(1000);
                Serial.println("State Error");
                break;
            }
    }
}
