// Include Libraries
#include <Arduino.h>
#include <MBR_analogRead.h>
#include <MBR_print.h>
#include <MBR_functions.h>
#include "utilities.h"
#include "LED.h"
#include "switch.h"
#include "Joystick.h"
#include "mbr_servo.h"
#include "f_print.h"
#include "wheel.h"

int cycle = 0;

const int joy_x_pin = A6;
const int joy_y_pin = A7;
const int joy_sw_pin = 6;

Joystick joy1(joy_x_pin, joy_y_pin, joy_sw_pin);

void setup()
{
  utilities_setup();
  LED_setup();
  wheel_setup();
  pinMode(6,INPUT_PULLUP);
}

void loop()
{

  // check online or offline
  // check mode
  // read sensors
  // transmit data
  
  // Read switch
  joySW = joy1.getSW();
  // joySW = !digitalRead(6);
  
  armState = checkArmState(joySW);
  
  // If new state selected, initlaize position
  if(initialize_pos == true)
  {
    degs[0] = moveServo(degs[0], target_x);
    degs[1] = moveServo(degs[1], target_y);
    LED_initializing(); 
    if(target_x == degs[0] && target_y == degs[1]) {initialize_pos = false;}
  }

  // If system unarmed, set parameters, initialize position if neccessary
  if(armState == false)
  {
    mode = 0;
    target_x = deg_min;
    target_y = deg_min;
    brightness = 0;
    mirror_state = false;
    if(inInitialPos() == false) {initialize_pos == true;}
    else{initialize_pos = false;}
  }
  
  // If system armed, check mode, set parameters, initialize position if neccessary
  if(armState == true)
  {
    mode = checkMode(joySW);
    if(mode == 1)
    {
      target_x = deg_mid;
      target_y = deg_mid;
      brightness = 255;
      mirror_state = false;
    }
  
   if(mode == 2)
    {
      target_x = 40;
      target_y = 120;
      brightness = 255;
      mirror_state = true;
    }

    if(mode == 3)
    {
      target_x = deg_mid;
      target_y = deg_mid;
      brightness = 255;
      mirror_state = false;
    }
    if(inInitialPos() == false) {initialize_pos == true;}
    else{initialize_pos = false;}
  }
  
  // Read joystick & add deadband [-1:1]
  // Joystick for relative positioning
  joy1_x = -joy1.getX();
  joy1_y = -joy1.getY();
  joy1_x = f_deadband(joy1_x, .25);
  joy1_y = f_deadband(joy1_y, .25);

  // Generate Step 
  step_x = joy1_x * 30;
  step_y = joy1_y * 30;

  // Generate Targets
  target_x = degs[0] + step_x;
  target_y = degs[1] + step_y;

  // Servo Next Steps
  degs[0] = moveServo(degs[0], target_x);
  degs[1] = moveServo(degs[1], target_y);







  cycle++;
  print_int("cycle", cycle);
  print1();
  
  delay(10);
}
