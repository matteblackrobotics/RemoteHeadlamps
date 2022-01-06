

// Include Libraries
#include <Arduino.h>
#include <MBR_analogRead.h>
#include <MBR_print.h>
#include <MBR_functions.h>
#include "utilities.h"
#include "LED.h"
#include "joySW.h"
#include "Joystick.h"
#include "mbr_servo.h"
#include "f_print.h"
#include "wheel.h"

int cycle = 0;

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
  
  // joySW = joy1.getSW();
  
  joySW = !digitalRead(6);
  
  armState = checkArmState(joySW);
  
  if(initialize == true)
  {
    
  }

  
  if(armState == false)
  {
    mode = 0;
    target_x = deg_min;
    target_y = deg_min;
    brightness = 0;
    mirror_state = false;
    initialize = true;
  }
  
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
    
  }
  
  cycle++;
  print_int("cycle", cycle);
  print1();
  
  delay(10);
}
