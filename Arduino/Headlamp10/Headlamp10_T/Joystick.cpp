// Source File
// What do all the functions do...

#include "Joystick.h"   // include local library
#include "Arduino.h"

// Constructor to configure data
// Class::Constructor
Joystick::Joystick(int pinX, int pinY, int pinSW)
{
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  
  _pinX = pinX;
  _pinY = pinY;
  _pinSW = pinSW; 
}

float volts_5 = 5000.0;               // mV input power
float cal = 0;                        // [mV] joystick calibration

float f_normAnalog(float analogIn, float Vin)
{
  analogIn = analogIn * (Vin/1023.0);           // convert to mV
  analogIn = analogIn/Vin;                      // convert to unit
  analogIn = analogIn + cal;                    // add calibration
  analogIn = analogIn - .5;                     // centers posiiton 
  analogIn = analogIn * 2;                      // range of 2 from [-1,1]     
  return analogIn;
}

bool Joystick::getSW()
{
  bool joySW = digitalRead(_pinSW);         // pullup
  joySW = !joySW;                           // flips boolean
  return joySW;
}

float Joystick::getPos(int pin)
{
  float pos = analogRead(pin);                  // read raw data
  pos = f_normAnalog(pos, volts_5);             // range of 2 from [-1,1]     
  return pos;
}

float Joystick::getX()
{
  float pos = analogRead(_pinX);                // read raw data
  pos = f_normAnalog(pos, volts_5);            // calibrate reading    
  return pos;
}

float Joystick::getY()
{
  float pos = analogRead(_pinY);                // read raw data
  pos = f_normAnalog(pos, volts_5);            // calibrate reading    
  return pos;
}
