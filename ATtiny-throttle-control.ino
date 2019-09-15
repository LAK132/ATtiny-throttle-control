// ATtiny85 pins:
// Pin 0 - I2C SDA, PWN (LED on Model B)
// Pin 1 - PWN (LED on Model A)
// Pin 2 - I2C SCK, Analog In
// Pin 3 - Analog In (also used for USB+ when USB is in use)
// Pin 4 - PWM, Analog (also used for USB- when USB is in use)
// Pin 5 - Analog In

const int TempSensorPin = 0;   // Pin 5 (Analog)
const int ControlValvePin = 1; // Pin 1

const unsigned long MaxOnTime = 10000; // Time in millis
const int CutOffTemp = 100; // Min analog value

bool valve_on = true;

void setup()
{
  pinMode(ControlValvePin, OUTPUT);
  digitalWrite(ControlValvePin, HIGH);
}

void loop()
{
  if (valve_on)
  {
    if (millis() > MaxOnTime ||
        analogRead(TempSensorPin) < CutOffTemp)
    {
      valve_on = false;
      digitalWrite(ControlValvePin, LOW);
    }
  }
}
