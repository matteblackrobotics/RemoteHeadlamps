
// Set up the structure of the class and include the functiones
#ifndef joystick_h
#define joystick_h

// Include libraries
// #include "Joystick.h" refernecing itself?
#include "Arduino.h"

float joy1_x;
float joy1_y;

class Joystick
{
  public:  // Functions you can call from another program

    // Constructor
    Joystick(int pinX, int pinY, int pinSW);

    // Methods
    // class functions that corrilate to .cpp functions
    bool getSW();
    float getX();
    float getY();
    float getPos(int pin);
      
  // data used within the class only
  // can be used in the cpp but cannot be called by any other sketch
  private: 
    int _pinX;
    int _pinY;
    int _pinSW;
    int Vin = 5000;               // mV input power
    float cal;                    // joystick calibration
};



#endif
