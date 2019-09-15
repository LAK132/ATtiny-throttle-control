// ATtiny85 pins:
// Pin 0 - I2C SDA, PWN (LED on Model B)
// Pin 1 - PWN (LED on Model A)
// Pin 2 - I2C SCK, Analog In
// Pin 3 - Analog In (also used for USB+ when USB is in use)
// Pin 4 - PWM, Analog (also used for USB- when USB is in use)
// Pin 5 - Analog In

#include <limits.h>

const int TempSensorPin = 0;            // Pin 5 (Analog)
const int ControlValvePin = 1;          // Pin 1
const unsigned long MaxOnTime = 10000;  // Time in millis
const int CutOffTemp = 100;             // Min analog value of temperature sensor
const int MaxReadings = 100;            // Number of temperature values to average

void setup()
{
  pinMode(ControlValvePin, OUTPUT);
  digitalWrite(ControlValvePin, HIGH);
}

bool valve_on = true;
int readings[MaxReadings] = {INT_MAX};
int reading_index = 0;

void loop()
{
  if (valve_on)
  {
    // Rolling average of temperature readings
    if (reading_index >= MaxReadings) reading_index = 0;
    readings[reading_index++] = analogRead(TempSensorPin);

    long readings_average = 0;
    for (int i = 0; i < MaxReadings; ++i)
      readings_average += readings[i];
    readings_average /= MaxReadings;

    if (millis() > MaxOnTime || readings_average < CutOffTemp)
    {
      valve_on = false;
      digitalWrite(ControlValvePin, LOW);
    }
  }
}
