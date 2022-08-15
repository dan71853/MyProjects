# 1 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\LEDStrip\\ESP\\LEDStrip.ino"
# 2 "c:\\Users\\somme\\Documents\\Git\\TestProjects\\FinalProjects\\LEDStrip\\ESP\\LEDStrip.ino" 2

// https://esphome.io/devices/nodemcu_esp32.html
const uint8_t LED_R = 14;
const uint8_t LED_G = 13;
const uint8_t LED_B = 5;
const uint8_t PWM_CH_R = 0;
const uint8_t PWM_CH_G = 1;
const uint8_t PWM_CH_B = 2;
const uint8_t LED_R_HEAD = 27;
const uint8_t LED_G_HEAD = 26;
const uint8_t LED_B_HEAD = 25;
const uint8_t PWM_CH_R_HEAD = 3;
const uint8_t PWM_CH_G_HEAD = 4;
const uint8_t PWM_CH_B_HEAD = 5;
const uint8_t S1 = 33; // Head- latching
const uint8_t S2 = 0; // Momentary
const uint8_t S3 = 35; // Potswitch

int red, green, blue; // red, green and blue values
unsigned long timer = millis();
const int freq = 8000;
const int resolution = 8;

boolean lightsOn = true;
boolean manualMode = false;
boolean headLightsMatch = false;

boolean manualModeChanged = false;
boolean modeChanged = false;
boolean headLightsMatchChange = false;
boolean lightsOnChanged = false;
unsigned long debouncingTimer = millis();
boolean deboundingTimerStarted = false;

uint8_t mode = 0;
uint8_t numberOfModes = 2;

void __attribute__((section(".iram1" "." "27"))) s1Pressed() {
    ;
    if (digitalRead(S1)) {
        lightsOn = true;
    } else {
        lightsOn = false;
    }
    lightsOnChanged = true;
    debouncingTimer = millis();
    deboundingTimerStarted = true;
}

void __attribute__((section(".iram1" "." "28"))) s2Pressed() {
    ;

    if (manualMode) {
        mode++;
        if (mode >= numberOfModes) {
            mode = 0;
        }
        modeChanged = true;
    } else {
        headLightsMatch = !headLightsMatch;
        if (!headLightsMatch) {
            s1Pressed();
        }
        headLightsMatchChange = true;
    }

    debouncingTimer = millis();
    deboundingTimerStarted = true;
}

void __attribute__((section(".iram1" "." "29"))) s3Pressed() {
    ;

    if (digitalRead(S3)) {
        manualMode = false;
    } else {
        manualMode = true;
        mode = 0;
    }
    manualModeChanged = true;
    debouncingTimer = millis();
    deboundingTimerStarted = true;
}

void setup() {
    Serial.begin(9600);
    // initial values (no significance)
    int red = 0;
    int blue = 0;
    int green = 0;

    analogWriteFrequency(freq); // Set the PWM frequency

    ledcSetup(PWM_CH_R, freq, resolution);
    ledcAttachPin(LED_R, PWM_CH_R);
    ledcSetup(PWM_CH_G, freq, resolution);
    ledcAttachPin(LED_G, PWM_CH_G);
    ledcSetup(PWM_CH_B, freq, resolution);
    ledcAttachPin(LED_B, PWM_CH_B);

    ledcSetup(PWM_CH_R_HEAD, freq, resolution);
    ledcAttachPin(LED_R_HEAD, PWM_CH_R_HEAD);
    ledcSetup(PWM_CH_G_HEAD, freq, resolution);
    ledcAttachPin(LED_G_HEAD, PWM_CH_G_HEAD);
    ledcSetup(PWM_CH_B_HEAD, freq, resolution);
    ledcAttachPin(LED_B_HEAD, PWM_CH_B_HEAD);
    headLightsOn();

    pinMode(S1, 0x05);
    attachInterrupt(S1, s1Pressed, 0x03);
    pinMode(S2, 0x05);
    attachInterrupt(S2, s2Pressed, 0x02);
    pinMode(S3, 0x05);
    attachInterrupt(S3, s3Pressed, 0x03);

    ledcWrite(PWM_CH_R, 0);
    ledcWrite(PWM_CH_G, 255);
    ledcWrite(PWM_CH_B, 0);
}

// Orange colour for eyes
void headLightsOff() {
    ledcWrite(PWM_CH_R_HEAD, 0x00);
    ledcWrite(PWM_CH_G_HEAD, 0x00);
    ledcWrite(PWM_CH_B_HEAD, 0x00);
}

void headLightsOn() {
    ledcWrite(PWM_CH_R_HEAD, 0x50);
    ledcWrite(PWM_CH_G_HEAD, 0x80);
    ledcWrite(PWM_CH_B_HEAD, 0x00);

    if (ledcRead(PWM_CH_R_HEAD) != 0x50)
        ledcWrite(PWM_CH_R_HEAD, 0x50);

    if (ledcRead(PWM_CH_G_HEAD) != 0x80)
        ledcWrite(PWM_CH_G_HEAD, 0x80);

    if (ledcRead(PWM_CH_B_HEAD) != 0x00)
        ledcWrite(PWM_CH_B_HEAD, 0x00);
}

void loop() {
    if (deboundingTimerStarted && millis() - debouncingTimer > 800) {
        deboundingTimerStarted = false;
        ;
    }
    if (manualModeChanged) {
        Serial.println("manualMode");
        Serial.println(manualMode);
        manualModeChanged = false;
    }
    if (modeChanged) {
        Serial.println("mode");
        Serial.println(mode);
        modeChanged = false;
    }
    if (headLightsMatchChange) {
        Serial.println("headLightsMatch");
        Serial.println(headLightsMatch);
        headLightsMatchChange = false;
    }
    if (lightsOnChanged) {
        Serial.println("lightsOn");
        Serial.println(lightsOn);
        if (lightsOn) {
            headLightsOn();
        } else {
            headLightsOff();
        }
        lightsOnChanged = false;
    }

    if (!lightsOn) {
        return;
    }

    if (manualMode) {
        switch (mode) {
            case 0:
                // Mode 0, all LEDS on
                red = 0xFF;
                green = 0xFF;
                blue = 0xFF;
                break;
            case 1:
                // Mode 1, Blue light
                red = 0x00;
                green = 0x50;
                blue = 0xA0;
                break;
            default:
                break;
        }

        ledcWrite(PWM_CH_R, red);
        ledcWrite(PWM_CH_G, green);
        ledcWrite(PWM_CH_B, blue);

        if (headLightsMatch) {
            ledcWrite(PWM_CH_R_HEAD, red);
            ledcWrite(PWM_CH_G_HEAD, green);
            ledcWrite(PWM_CH_B_HEAD, blue);
        }
    } else {
        // protocol expects data in format of 4 bytes
        //(xff) as a marker to ensure proper synchronization always
        // followed by red, green, blue bytes
        if (Serial.available() >= 4) {
            timer = millis();
            if (Serial.read() == 0xff) {
                red = (Serial.read());
                green = (Serial.read());
                blue = (Serial.read());
            }
        }
        if (millis() - timer > 10000) {
            red = 0;
            green = 0;
            blue = 0;
        }

        // finally control led brightness through pulse-width modulation
        ledcWrite(PWM_CH_R, red);
        ledcWrite(PWM_CH_G, green);
        ledcWrite(PWM_CH_B, blue);

        if (headLightsMatch) {
            ledcWrite(PWM_CH_R_HEAD, red);
            ledcWrite(PWM_CH_G_HEAD, green);
            ledcWrite(PWM_CH_B_HEAD, blue);
        }
        // ledcWrite(PWM_CH_R_HEAD, 0x66);
        // ledcWrite(PWM_CH_G_HEAD, 0x80);
        // ledcWrite(PWM_CH_B_HEAD, 0x00);
        delay(10); // just to be safe
    }
}
