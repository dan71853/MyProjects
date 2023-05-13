#define SERVO_PIN 4

uint8_t step = 20;
uint8_t numOfSteps = 5;
void setup() {
    // pinMode(0, OUTPUT);
    // TCCR0A |= 0b10000011;  //Output on pin 1, fast PWM
    // TCCR0B |= 0b00001100;  //Fast PWM TOP = OCRA, 256 clock porescaler
    // OCR0A = 128;

    pinMode(SERVO_PIN, OUTPUT);
}

void loop() {
    for (uint8_t angle = 0; angle < 180; angle += step) {
        for (uint8_t i = 0; i < numOfSteps; i++) {
            setAngle(angle);
        }
    }
    for (uint8_t angle = 180; angle > 0; angle -= step) {
        for (uint8_t i = 0; i < numOfSteps; i++) {
            setAngle(angle);
        }
    }
}


void setAngle(uint16_t angle) {
    uint16_t onTime = 5.5555 * angle + 1000;  //uss
    uint16_t offTime = 2000 - onTime;
    digitalWrite(SERVO_PIN, 1);
    delayMicroseconds(onTime);
    digitalWrite(SERVO_PIN, 0);
    delayMicroseconds(offTime);
    delay(18);
}