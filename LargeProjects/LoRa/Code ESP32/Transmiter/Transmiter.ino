// #define MAX_RANGE
#define DEBUG

#define SEND_DELAY 3000

#include <LoRa.h>

#define SPI_SS_PIN 4
#define SX1276_RESET_PIN 27
#define SX1276_INTERRUPT_PI 14

#define ONBOARD_LED 2

#define FREQUENCY 433E6
// #define FREQUENCY 915E6
#ifdef MAX_RANGE
// #define LORA_TXPWR 20
// #define LORA_SP 7
// #define LORA_BW 125E3
#define LORA_TXPWR 20
#define LORA_SP 12
#define LORA_BW 500E3
#else
#define LORA_TXPWR 17
#define LORA_SP 7
#define LORA_BW 125E3
#endif

typedef enum LED_MODE {
  LED_START,        //Startup is 3 short blinks
  LED_SETUP_ERROR,  //1 short then 1 long, repeats
  LED_FLASH,

} LED_MODE;
void flashLed(LED_MODE mode, int pin = ONBOARD_LED);

void setup() {
#ifdef DEBUG
  //Enable serial monitor
  //Ensure DEBUG is defined
  Serial.begin(115200);
  Serial.println("Starting Transmiter Module");
#endif

  //Setup onboard led
  pinMode(ONBOARD_LED, OUTPUT);

  flashLed(LED_START);

  // Setup the other required pins
  LoRa.setPins(SPI_SS_PIN, SX1276_RESET_PIN, SX1276_INTERRUPT_PI);

  if (!LoRa.begin(FREQUENCY)) {
    flashLed(LED_SETUP_ERROR);  //Blocking
  }

  LoRa.setTxPower(LORA_TXPWR);
  LoRa.setSpreadingFactor(LORA_SP);
  LoRa.setSignalBandwidth(LORA_BW);
}


uint8_t loraBuffer[2] = { 69, 0 };

void loop() {
  //Sent a message every 5 seconds

  LoRa.beginPacket();
  LoRa.write(loraBuffer, 2);
  LoRa.endPacket();
#ifdef DEBUG
  Serial.println("Packet sent");
  Serial.print("buffer: ");
  Serial.println(loraBuffer[1]);

#endif

  loraBuffer[1] = 1 - loraBuffer[1];

  flashLed(LED_FLASH);
  delay(SEND_DELAY);
}



/**
 * Blocking method that flash led based on mode
 */
void flashLed(LED_MODE mode, int pin) {
  switch (mode) {
    case LED_START:
      for (uint8_t i = 0; i < 3; i++) {
        digitalWrite(pin, HIGH);
        delay(200);
        digitalWrite(pin, LOW);
        delay(200);
      }
      break;

    case LED_SETUP_ERROR:
#ifdef DEBUG
      Serial.println("Starting LoRa failed!");
#endif
      while (1) {
        digitalWrite(pin, HIGH);
        delay(500);
        digitalWrite(pin, LOW);
        delay(500);
        digitalWrite(pin, HIGH);
        delay(2000);
        digitalWrite(pin, LOW);
        delay(500);
      }
      break;

    case LED_FLASH:
      digitalWrite(pin, HIGH);
      delay(200);
      digitalWrite(pin, LOW);
      delay(200);
      break;

    default:
#ifdef DEBUG
      Serial.println("Error: Unknown LED Flash mode");
#endif
      //Error, set LED high
      digitalWrite(pin, HIGH);
      break;
  }
}