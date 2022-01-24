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

void printJoystick()
{
  printInt("sw", joySW);
  printInt("hold_count", holdCount);
  printInt("arm", armState);
  printInt("mode", mode);
  printInt("init", initializing);
}

void printGimbal()
{
  printInt("stepX", stepX);
  printInt("stepY", stepY);
  printInt("targetX", targetX);
  printInt("x1", degs[0]);
  printInt("x2", degs[2]);
  printInt("targetY", targetY);
  printInt("y1", degs[1]);
  printInt("y2", degs[3]);
}

void printWheel()
{
  printFloat("potVolt", potVolt);
  printFloat("potVal", potVal);
  printFloat("wheelRot", wheelRot);
}


  
#endif
