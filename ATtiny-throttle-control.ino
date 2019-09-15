// ATtiny85 pins:
// Pin 0 - I2C SDA, PWM (LED on Model B)
// Pin 1 - PWM (LED on Model A)
// Pin 2 - I2C SCK, Analog In
// Pin 3 - Analog In (also used for USB+ when USB is in use)
// Pin 4 - PWM, Analog (also used for USB- when USB is in use)
// Pin 5 - Analog In

#include <limits.h>
#include <avr/sleep.h>

const int TempSensorPin = 0;            // Pin 5 (Analog)
const int ControlValvePin = 1;          // Pin 1
const unsigned long MaxOnTime = 60000;  // Time in millis
const int CutOffTemp = 100;             // Min analog value of temperature sensor
const int MaxReadings = 100;            // Number of temperature values to average

long rollingAverage()
{
  static int readings[MaxReadings] = {INT_MAX};
  static int reading_index = 0;
  if (reading_index >= MaxReadings) reading_index = 0;
  readings[reading_index++] = analogRead(TempSensorPin);

  long readings_average = 0;
  for (int i = 0; i < MaxReadings; ++i)
    readings_average += readings[i];
  readings_average /= MaxReadings;
}

void deepSleep()
{
  _SFR_BYTE(ADCSRA) &= ~_BV(ADEN);      // Switch ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Set sleep mode
  sleep_enable();
  sleep_mode();                         // System actually sleeps here
  sleep_disable();                      // System continues execution here when watchdog timed out
  _SFR_BYTE(ADCSRA) |= _BV(ADEN);       // Switch ADC on
}

bool valve_on = true;

void setup()
{
  // Init the rolling average readings
  for (int i = 0; i < MaxReadings; ++i) rollingAverage();

  valve_on = rollingAverage() > CutOffTemp;

  pinMode(ControlValvePin, OUTPUT);
  digitalWrite(ControlValvePin, valve_on ? HIGH : LOW);
}

void loop()
{
  if (valve_on)
  {
    if (millis() > MaxOnTime || rollingAverage() < CutOffTemp)
    {
      valve_on = false;
      digitalWrite(ControlValvePin, LOW);
    }
  }
  else deepSleep();

  delay(10);
}
