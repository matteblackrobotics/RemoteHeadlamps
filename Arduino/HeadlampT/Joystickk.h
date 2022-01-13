#ifndef Joystickk_h
#define Joystickk_h

#include "LED.h"


// ----------------------------------------- Joystick --------------------------------------- //
#define joyXPin A6
#define joyYPin A7
#define joySWPin 6
Joystick joy(joyXPin, joyYPin, joySWPin); // initialize - class: Joystick - object: joy1

float joyX;
float joyY;
bool joySW;
bool joySWLast;
int joyMax = 1;
int joyMin = -1;


// -------------------------------------- button press --------------------------------- //
const int modeMax = 2;       // [0,1,2]
int mode = modeMax;                 // spotlight, mirror, auto
bool mirrorState = false;    // do lamps mirror
bool initializing = true;      // do lamps need to move to initial position?

int holdCount = 0;
const int holdThresh1 = 20;
bool armState = false;                // stow or operational, [0,1]
const int armStateMax = 1;



// Is the system armed or not
bool checkArmState(bool sw)
  {
    if(sw == LOW) {holdCount = 0;}
    if(sw == HIGH) 
    {
      ledBrightYellow();
      holdCount++;
    }
    if(holdCount == holdThresh1) 
    {
      armState = !armState;
      holdCount = 0;
    }
    if(armState == false) {mode = 0;}
    return armState;
  }


int checkMode(bool sw, bool swLast)
  {
    if(sw == HIGH)
    {
      ledBrightYellow();
      if(sw != swLast)
      {
        mode++;
        initializing = true;
        if(mode > modeMax) {mode = 0;}
      }
    }
    swLast = sw;
    return mode;
  }

  
#endif
