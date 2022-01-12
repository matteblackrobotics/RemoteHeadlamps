#ifndef Joystickk_h
#define Joystickk_h

#include "LED.h"


// ----------------------------------------- Joystick --------------------------------------- //
#define joy_x_pin A6
#define joy_y_pin A7
#define joy_sw_pin 6
Joystick joy1(joy_x_pin, joy_y_pin, joy_sw_pin); // initialize - class: Joystick - object: joy1

float joy1_x;
float joy1_y;
bool joy1_sw;
bool joy1_sw_last;
int joy_max = 1;
int joy_min = -1;


// -------------------------------------- button press --------------------------------- //
const int mode_max = 2;       // [0,1,2]
int mode = mode_max;                 // spotlight, mirror, auto
bool mirror_state = false;    // do lamps mirror
bool initial_pos = true;      // do lamps need to move to initial position?

int hold_count = 0;
const int hold_thresh1 = 20;
bool arm_state = false;                // stow or operational, [0,1]
const int arm_state_max = 1;



// Is the system armed or not
bool checkArmState(bool sw)
  {
    if(sw == LOW) {hold_count = 0;}
    if(sw == HIGH) 
    {
      LEDBrightYellow();
      hold_count++;
    }
    if(hold_count == hold_thresh1) 
    {
      arm_state = !arm_state;
      hold_count = 0;
    }
    if(arm_state == false) {mode = 0;}
    return arm_state;
  }


int checkMode(bool sw, bool sw_last)
  {
    if(sw == HIGH)
    {
      LEDBrightYellow();
      if(sw != sw_last)
      {
        mode++;
        initial_pos = true;
        if(mode > mode_max) {mode = 0;}
      }
    }
    sw_last = sw;
    return mode;
  }

  
#endif
