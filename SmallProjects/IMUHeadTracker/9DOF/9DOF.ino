#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include <Joystick.h>

// Create Joystick
Joystick_ Joystick;

const int pinToButtonMap = 9;
int lastButtonState = 0;
int tempNum=0;


/* Assign a unique ID to the sensors */
Adafruit_9DOF  dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

#define ARRAYSIZE 10
float p[ARRAYSIZE],y[ARRAYSIZE],r[ARRAYSIZE];
int counter = 0;
void setup(){
  Serial.begin(115200);
  delay(3000);
  Serial.println(F("Adafruit 9 DOF Pitch/Roll/Heading Example")); Serial.println("");

  if (!accel.begin())  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while (1);
  }
  if (!mag.begin())  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }

  pinMode(pinToButtonMap, INPUT_PULLUP);
  Joystick.setXAxisRange(-127, 127);
  Joystick.setYAxisRange(-127, 127);
  Joystick.setZAxisRange(-127, 127);
	// Initialize Joystick Library
	Joystick.begin();
}


void loop(){
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  
  
  
  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))  {
    /* 'orientation' should have valid .roll and .pitch fields */
    r[counter] = orientation.roll;
    p[counter] = orientation.pitch;
 
  }

  /* Calculate the heading using the magnetometer */
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))  {
    /* 'orientation' should have valid .heading data now */
    y[counter] = orientation.heading;
    
  }
  counter++;
  if(counter>ARRAYSIZE){
    counter = 0;
  }


  
//   Serial.print(F("Roll: "));
    Serial.print(mean(r));
    Serial.print(F(", "));
//    Serial.print(F("Pitch: "));
    Serial.print(mean(p));
    Serial.print(F(", "));
//    Serial.print(F("Heading: "));
    Serial.print(mean(y));
    Serial.print(F(", "));
  Serial.println(F(""));
  delay(50);


 Joystick.setXAxis(mean(p));
 Joystick.setYAxis(mean(y));
 Joystick.setZAxis(mean(r));


  // Read pin values
	int currentButtonState = !digitalRead(pinToButtonMap);
	if (currentButtonState != lastButtonState)
	{
	Joystick.setButton(0, currentButtonState);
	lastButtonState = currentButtonState;
  tempNum=0;
	}



}

float mean(float*array){
  float total = 0;
  for (int i = 0; i < ARRAYSIZE; i++)  {
    total+=array[i];
  }
  return total/float(ARRAYSIZE);
}
