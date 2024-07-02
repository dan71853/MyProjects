// ESP32 Touch Test
// Just test touch pin - Touch0 is T0 which is on GPIO 4.

void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
}

void loop() {
  Serial.print(touchRead(32));  // 60
  Serial.print(", ");
  Serial.println(touchRead(12));  // 73
  delay(500);
}
