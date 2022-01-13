#ifndef PrintOut_h
#define PrintOut_h

#include <Arduino.h>
#include <math.h>
#include "Lamp.h"

void printInt(String a, int b)
{
  Serial.print(" ");
  Serial.print(a);
  Serial.print(": ");
  Serial.print(b); 
  Serial.print("  ");
}


void printFloat(String a, float b)
{
  Serial.print(" ");
  Serial.print(a);
  Serial.print(": ");
  Serial.print(b); 
  Serial.print("  ");
}


void printBool(String a, bool b)
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

void printLn()
{
  Serial.println(" ");
}

void print1()
  {
  printBool("sw", joySW);
  printInt("hold_count", holdCount);
  Serial.print("  arm_" ); Serial.print(armState); Serial.print(" ");
  Serial.print("  mode_" ); Serial.print(mode); Serial.print(" ");
  Serial.print("  mirror_" ); Serial.print(mirrorState); Serial.print(" ");
  Serial.print("  initializing_"); Serial.print(initializing); Serial.print(" ");
  Serial.print("  joyX_" ); Serial.print(joyX); Serial.print(" ");  
  Serial.print("  joyY_" ); Serial.print(joyY); Serial.print(" ");  
  Serial.print("  joySW_" ); Serial.print(joySW); Serial.print(" "); 
  Serial.print("  bri_" ); Serial.print(lampBrightness); Serial.print(" ");
  Serial.print("  step_x_" ); Serial.print(stepX); Serial.print(" ");
  Serial.print("  step_y_" ); Serial.print(stepY); Serial.print(" ");
  Serial.print("  target,x1,x2_" ); Serial.print(targetX); Serial.print(" "); Serial.print(degs[0]); Serial.print(" "); Serial.print(degs[2]); Serial.print(" "); 
  Serial.print("  target,y1,y2_" ); Serial.print(targetY); Serial.print(" "); Serial.print(degs[1]); Serial.print(" "); Serial.print(degs[3]); Serial.print(" ");
  Serial.print("  pot_val_" ); Serial.print(potVal); Serial.print(" ");
  Serial.println("");
  }

void print2()
  {
  Serial.print( "joySW: " ); Serial.print(joySW); Serial.print(" "); 
  Serial.print( "  x: " ); 
  Serial.print(joyX); Serial.print(" ");
  Serial.print(stepX); Serial.print(" ");
  Serial.print(targetX); Serial.print(" ");
  Serial.print(degs[0]); Serial.print(" "); 
  Serial.print( "  y: " ); 
  Serial.print(joyY); Serial.print(" ");  
  Serial.print(stepY); Serial.print(" ");
  Serial.print(targetY); Serial.print(" "); 
  Serial.print(degs[1]); Serial.print(" "); 
  }

void print3()
  {
  Serial.print(targetX); Serial.print(" ");
  Serial.print(degs[0]); Serial.print(" "); 
  Serial.print(targetY); Serial.print(" "); 
  Serial.print(degs[1]); Serial.print(" "); 
  }


  
#endif
