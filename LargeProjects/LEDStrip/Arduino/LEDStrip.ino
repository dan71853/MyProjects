// #define UNO
#define UNO

#ifdef UNO
const uint8_t LED_R = 9;
const uint8_t LED_G = 10;
const uint8_t LED_B = 11;
#endif
#ifdef PROMICRO
const uint8_t LED_R = 9;
const uint8_t LED_G = 10;
const uint8_t LED_B = 5;
#endif
int red, green, blue;  // red, green and blue values
unsigned long time = millis();
void setup() {
    Serial.begin(9600);
    // initial values (no significance)
    int red = 255;
    int blue = 255;
    int green = 255;

//     analogWrite(LED_R, 200);
//    analogWrite(LED_G, 0);
//    analogWrite(LED_B, 200);
//    while(1);
}

void loop() {  // protocol expects data in format of 4 bytes
    //(xff) as a marker to ensure proper synchronization always
    // followed by red, green, blue bytes
    if (Serial.available() >= 4) {
        time = millis();
        if (Serial.read() == 0xff) {
            red = Serial.read();
            green = Serial.read();
            blue = Serial.read();
        }
    }
    if (millis() - time > 10000) {
        red = 0;
        green = 0;
        blue = 0;
    }
    // finally control led brightness through pulse-width modulation
    analogWrite(LED_R, red);
    analogWrite(LED_G, green);
    analogWrite(LED_B, blue);
    delay(10);  // just to be safe
}
