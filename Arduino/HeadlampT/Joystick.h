#ifndef Joystick_h
#define Joystick_h

#include "LED.h"


// ----------------------------------------- Joystick --------------------------------------- //
#define joyXPin A6
#define joyYPin A7
#define joySWPin 6
// Joystick joy(joyXPin, joyYPin, joySWPin); // initialize - class: Joystick - object: joy1

float joyX;
float joyY;
bool joySW;
bool joySWLast;


// -------------------------------------- button press --------------------------------- //
const int modeMax = 2;       // [0,1,2]
int mode = modeMax;                 // spotlight, mirror, auto
bool mirrorState = false;    // do lamps mirror
bool initializing = true;      // do lamps need to move to initial position?

int holdCount = 0;
const int holdThresh1 = 10;
bool armState = false;                // stow or operational, [0,1]
bool armStateLast;

void setJoyX(float _joyX)
{
  joyX = _joyX;
}

float getJoyX()
{
  return joyX;
}



void joystickSetup()
{
  pinMode(joySWPin, INPUT_PULLUP);
}

float normalizeInput(float input)
{
  input = input/1023.0;   // convert to [0:1]
  input = input - .5;     // center across 0
  input = input * 2;      // increase range [-1:1]
  return input;
}

float readJoyX()
{
  float x = analogRead(joyXPin);
  x = normalizeInput(x);
  return x;
}

float readJoyY()
{
  float y = analogRead(joyYPin);
  y = normalizeInput(y);
  return y;
}


bool checkJoySW() 
{
  bool sw = digitalRead(joySWPin);
  sw = !sw; //reverse for pullup switch
  return sw;
}


// Is the system armed or not
bool checkArmState(bool sw1)
  {
    if(sw1 == LOW) 
    {
      holdCount = 0;
    }
    if(sw1 == HIGH) 
    {
      ledBrightYellow();              // not flashing yellow when button pressed...
      holdCount++;
    }
    if(holdCount >= holdThresh1) 
    {
      armState = !armState;
      holdCount = 0;
      initializing == true;
    }
    return armState;
  }

// What is the mode
// @param sw (switch state)
// @param swLast (last switch state)
// @return mode
int checkMode()
  {
    if(joySW == HIGH)
    {
      ledBrightYellow();
      if(joySW != joySWLast)
      {
        mode++;
        initializing = true;
        if(mode > modeMax) {mode = 0;}
      }
    }
    return mode;
  }
#endif