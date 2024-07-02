int THRESHOLD_LEFT = 60;  // Sensitivity threshold
int THRESHOLD_RIGHT = 73;  // Sensitivity threshold

RTC_DATA_ATTR int bootCount = 0;  // Number of reboots

touch_pad_t touchPin;  // GPIO pin that triggered the wake-up

void setup() {


  Serial.begin(115200);  // Start serial communication at 115200 baud rate

  ++bootCount;  // Add 1 to the current value of bootCount

  Serial.println("Boot number: " + String(bootCount));  // Print the value of bootCount on the serial monitor

  delay(1000);  //Take some time to open up the Serial Monitor

  touchPin = esp_sleep_get_touchpad_wakeup_status();  // Store which touch sensor was activated from wake-up data

  if (touchPin == TOUCH_PAD_NUM9) {

    Serial.println("Touch detected on GPIO 32, Left button");

  }  // Check which touch sensor triggered the wake-up and display its corresponding GPIO pin number

  else if (touchPin == TOUCH_PAD_NUM5) {

    Serial.println("Touch detected on GPIO 12, Right button");

  }

  else

  {

    Serial.println("Wakeup not by touchpad");
  }

  touchSleepWakeUpEnable(T9, THRESHOLD_LEFT);  
  touchSleepWakeUpEnable(T5, THRESHOLD_RIGHT); 

  Serial.println("Preparing to sleep now");  // Print when the ESP is about to go into deep sleep mode

  Serial.flush();  // Make sure the serial port is empty

  esp_deep_sleep_start();  // Enable the deep sleep mode
}

void loop() {

  //This will never be reached
}