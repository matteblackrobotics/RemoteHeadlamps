#ifndef PrintOut_h
#define PrintOut_h

#include <Arduino.h>
#include <math.h>
#include "Lamp.h"

void print_int(String a, int b)
{
  Serial.print(" ");
  Serial.print(a);
  Serial.print(": ");
  Serial.print(b); 
  Serial.print("  ");
}


void print_float(String a, float b)
{
  Serial.print(" ");
  Serial.print(a);
  Serial.print(": ");
  Serial.print(b); 
  Serial.print("  ");
}


void print_bool(String a, bool b)
{
  if(b == true)
  {
    Serial.print(" ");
    Serial.print(a);
    Serial.print(": true ");
  }

  if(b == false)
  {
    Serial.print(" ");
    Serial.print(a);
    Serial.print(": false ");
  }
}

void print_ln()
{
  Serial.println(" ");
}

  void print1()
  {
  print_bool("sw", joy1_sw);
  print_int("hold_count", hold_count);
  Serial.print("  arm_" ); Serial.print(arm_state); Serial.print(" ");
  Serial.print("  mode_" ); Serial.print(mode); Serial.print(" ");
  Serial.print("  mirror_" ); Serial.print(mirror_state); Serial.print(" ");
  Serial.print("  initial_pos_"); Serial.print(initial_pos); Serial.print(" ");
  Serial.print("  joyX_" ); Serial.print(joy1_x); Serial.print(" ");  
  Serial.print("  joyY_" ); Serial.print(joy1_y); Serial.print(" ");  
  Serial.print("  joySW_" ); Serial.print(joy1_sw); Serial.print(" "); 
  Serial.print("  bri_" ); Serial.print(lamp_brightness); Serial.print(" ");
  Serial.print("  step_x_" ); Serial.print(step_x); Serial.print(" ");
  Serial.print("  step_y_" ); Serial.print(step_y); Serial.print(" ");
  Serial.print("  target,x1,x2_" ); Serial.print(target_x); Serial.print(" "); Serial.print(degs[0]); Serial.print(" "); Serial.print(degs[2]); Serial.print(" "); 
  Serial.print("  target,y1,y2_" ); Serial.print(target_y); Serial.print(" "); Serial.print(degs[1]); Serial.print(" "); Serial.print(degs[3]); Serial.print(" ");
  Serial.print("  pot_val_" ); Serial.print(pot_val); Serial.print(" ");
  Serial.println("");
  }

void print2()
  {
  Serial.print( "joySW: " ); Serial.print(joy1_sw); Serial.print(" "); 
  Serial.print( "  x: " ); 
  Serial.print(joy1_x); Serial.print(" ");
  Serial.print(step_x); Serial.print(" ");
  Serial.print(target_x); Serial.print(" ");
  Serial.print(degs[0]); Serial.print(" "); 
  Serial.print( "  y: " ); 
  Serial.print(joy1_y); Serial.print(" ");  
  Serial.print(step_y); Serial.print(" ");
  Serial.print(target_y); Serial.print(" "); 
  Serial.print(degs[1]); Serial.print(" "); 
  }

void print3()
  {
  Serial.print(target_x); Serial.print(" ");
  Serial.print(degs[0]); Serial.print(" "); 
  Serial.print(target_y); Serial.print(" "); 
  Serial.print(degs[1]); Serial.print(" "); 
  }


  
#endif
