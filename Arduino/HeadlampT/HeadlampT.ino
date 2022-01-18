// main
// /dev/cu.usbserial-14320
// function branch
// explore public and private variabales
// explore functions

// standard libraries
// #include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include "RF24.h"

// internal libraries
#include "Arduino.h"
#include "Functions.h"
#include "Joystick.h"
#include "Utilities.h"
#include "Wheel.h"
#include "LED.h"
#include "Radio.h"
#include "Gimbal.h"
#include "PrintOut.h"
#include "Lamp.h"
#include "Timer.h"


// --------------------------- VOID SETUP -------------------- //
// --------------------------- VOID SETUP -------------------- //
void setup() 
{
 utilitiesSetup();
 joystickSetup();
 ledSetup();
 wheelSetup();
 radioSetup();
 timerSetup();
}


// -------------------------------- VOID LOOP ----------------------------- //
// -------------------------------- VOID LOOP ----------------------------- //
void loop() 
{
  if(checkTime(startTime1, duration1) == true) 
  {
    startTime1 = millis();
    ledWhite();
    delay(10);
  }

  joySW = readJoySW();                       // read joystick button
  armState = checkArmState(joySW);           // long button press?

  // -------------------- initialize position ---------------------- //
  // If new state is selected
  if(initializing == true)
  {
    initializeState();
  }

    
  // ---------------------------- Offline ---------------------------------  //
  if(armState == 0)    
  {
    targetX = degMin;
    targetY = degMax;
    lampBrightness = 0;
    mirrorState = false;
    initializing = true;
    mode = modeMax;
    ledLightBlue();
  }

  // ----------------------------- Online ------------------------------ //
  if(armState == 1)
  {
    mode = checkMode(joySW, joySWLast); 
    joySWLast = joySW;
        
    if(mode == 0) // Spotlight          
    {
      targetX = degMid;
      targetY = degMid;
      lampBrightness = 255;
      mirrorState = false; 
    }
    
    if(mode == 1) // Mirror         
    {
      targetX = 40;
      targetY = 120;
      lampBrightness = 255;
      mirrorState = true;
    }

    if(mode == 2) // Auto         
    {
      targetX = degMid;
      targetY = degMid;
      lampBrightness = 255;
      mirrorState = false;
    }

    if(initializing == false)
    {
    // -------------------- Joysitck Modes ---------------------- //
        if(mode == 0) // Spotlight
        {
          ledRed();
        }
        
        if(mode == 1) // mirror state
        {
          ledGreen();
        }    
        
        if(mode == 0 || mode == 1)
        {
          // Read joystick
          joyX = -readJoyX();  // [-1:1]
          joyY = -readJoyY();
          
          // Deadband for no motion
          joyX = addDeadband(joyX, .25);
          joyY = addDeadband(joyY, .25);
      
          // calculate next step size
          stepX = joyX * 30.0;  // max joystick = 5 steps
          stepY = joyY * 30.0;

          // new target
          targetX = degs[0] + stepX;
          targetY = degs[1] + stepY;

          // move servo to next step
          degs[0] = moveServo(targetX, degs[0]);
          degs[1] = moveServo(targetY, degs[1]);
        }
      
 
         // ------------------- Auto Mode -------------------- //
        if(mode == 2)
        {
          mirrorState = false;
          wheelRot = readWheelRot() - wheelCal;         // wheel position with calibration 
          lampPos = (wheelRot/wheelRotMax) * 90.0;      // [-90:90] lamp position from forward 
          targetX = lampPos + degMid;                  // abosolute positioning from 90
          degs[0] = moveServo(targetX, degs[0]);
          
          ledBlue();
          //printFloat("wheelCal", wheelCal);  
          //printFloat("wheelRot", wheelRot);    
          //printFloat("lampPos", wheelRot);
          //printFloat("targetX", targetX);
        }
      } 
    } // end Online
    
  
  dataPrep();
  dataTransmit();
  // ledOff();
  print1();
  // Serial.println(lampBrightness);
  delay(delay1);
}
