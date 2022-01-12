// standard libraries
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include "RF24.h"

// internal libraries
#include "Arduino.h"
#include "Functions.h"
#include "Joystick.h"
#include "Joystickk.h"
#include "Utilities.h"
#include "Wheel.h"
#include "LED.h"
#include "Radio.h"
#include "Gimbal.h"
#include "PrintOut.h"
#include "Lamp.h"


// --------------------------- VOID SETUP -------------------- //
// --------------------------- VOID SETUP -------------------- //
void setup() 
{
 utilities_setup();
 LED_setup();
 wheel_setup();
 radio_setup();
}


// -------------------------------- VOID LOOP ----------------------------- //
// -------------------------------- VOID LOOP ----------------------------- //
void loop() 
{
  joy1_sw = joy1.getSW();                       // read joystick button
  arm_state = checkArmState(joy1_sw);           // long button press?

  // -------------------- initialize position ---------------------- //
  // If new state is selected
  if(initial_pos == true)
  {
    degs[0] = f_moveServo(target_x, degs[0]);
    degs[1] = f_moveServo(target_y, degs[1]);
    LEDYellow();
    if(target_x == degs[0] && target_y == degs[1]) {initial_pos = false;} 
  }

    
  // ---------------------------- Offline ---------------------------------  //
  if(arm_state == 0)    
  {
    target_x = deg_min;
    target_y = deg_max;
    lamp_brightness = 0;
    mirror_state = false;
    initial_pos = true;
    mode = mode_max;
    LEDPurple();
  }

  // ----------------------------- Online ------------------------------ //
  if(arm_state == 1)
  {
    mode = checkMode(joy1_sw, joy1_sw_last); 
    joy1_sw_last = joy1_sw;
        
    if(mode == 0) // Spotlight          
    {
      target_x = deg_mid;
      target_y = deg_mid;
      lamp_brightness = 255;
      mirror_state = false; 
    }
    
    if(mode == 1) // Mirror         
    {
      target_x = 40;
      target_y = 120;
      lamp_brightness = 255;
      mirror_state = true;
    }

    if(mode == 2) // Auto         
    {
      target_x = deg_mid;
      target_y = deg_mid;
      lamp_brightness = 255;
      mirror_state = false;
    }

    if(initial_pos == false)
    {
    // -------------------- Joysitck Modes ---------------------- //
        if(mode == 0) // Spotlight
        {
          LEDRed();
        }
        
        if(mode == 1) // mirror state
        {
          LEDGreen();
        }    
        
        if(mode == 0 || mode == 1)
        {
          // Read joystick
          joy1_x = -joy1.getX();  // [-1:1]
          joy1_y = -joy1.getY();
          
          // Deadband for no motion
          joy1_x = f_deadband(joy1_x, .25);
          joy1_y = f_deadband(joy1_y, .25);
      
          // calculate next step size
          step_x = joy1_x * 30.0;  // max joystick = 5 steps
          step_y = joy1_y * 30.0;

          // new target
          target_x = degs[0] + step_x;
          target_y = degs[1] + step_y;

          // move servo to next step
          degs[0] = f_moveServo(target_x, degs[0]);
          degs[1] = f_moveServo(target_y, degs[1]);
        }
      
 
         // ------------------- Auto Mode -------------------- //
        if(mode == 2)
        {
          mirror_state = false;
          wheel_rot = readWheelRot() - wheel_cal;         // wheel position with calibration 
          lamp_pos = (wheel_rot/wheel_rot_max) * 90.0;      // [-90:90] lamp position from forward 
          target_x = lamp_pos + deg_mid;                  // abosolute positioning from 90
          degs[0] = f_moveServo(target_x, degs[0]);
          
          LEDBlue();
          print_float("wheel_cal", wheel_cal);  
          print_float("wheel_rot", wheel_rot);    
          print_float("lamp_pos", wheel_rot);
          print_float("target_x", target_x);
        }
      } 
    } // end Online
    
  
  // --------------------- Data Prep ---------------------------// 
  if(mirror_state == true)       // X Lamps Mirror
  {
    int a = 90 - degs[0];
    degs[2] = 90 + a;
  }
  else {degs[2] = degs[0];}      // X Lamps Follow
  degs[3] = degs[1];             // Y Lamps Follow


  // Bound servo degree
  for(int i=0; i<size_degs; i++)
  {
    degs[i] = f_bound(degs[i], deg_min, deg_max);
  }
  
  // Add mechanical 0s
 for(int i=0; i<size_degs; i++)
  {
    degs[i] = degs[i] + mech0s[i];
  }
  
  
  // ---------- populate & transmit data packet ------- //
  // send servo data, pot data, switch data
  // transmit to both
  data_t.joy_sw = joy1_sw;          
  data_t.brightness = lamp_brightness;

  // transmit to lamp 1
  data_t.servo_x = degs[0];   // servo_x1_degree
  data_t.servo_y = degs[1];   // servo__y1_degree
  myRadio.openWritingPipe(address1_TR);   // Lamp 1
  myRadio.write(&data_t, sizeof(data_t));

  // transmit to lamp 2
  data_t.servo_x = degs[2];   // servo_x2_degree
  data_t.servo_y = degs[3];   // servo_y2_degree
  myRadio.openWritingPipe(address2_TR);   // Lamp 2
  myRadio.write(&data_t, sizeof(data_t));

  LEDOff();
  print1();
  delay(0);
}


// --------------------------- END VOID LOOP ------------------------ //
// --------------------------- END VOID LOOP ------------------------ //


int f_moveServo(int target_deg, int deg)
{
  if(target_deg > deg)                          // move positive
  {
    if(target_deg - deg >= 10) {deg = deg + step_max;}  // big step
    else{deg = deg + step_min;}                          // small step
  }

  if(target_deg < deg)                          // move negative
  {
    if(deg - target_deg >= 10) {deg = deg - step_max;}  // big step
    else{deg = deg - step_min;}                            // small step
  }
  return deg;                                   // updated servo degrees
}
