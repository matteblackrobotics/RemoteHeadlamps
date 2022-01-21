// functionBranch

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

bool changeArmState = false;

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

// check switch hold to switch between armed and disarmed
// check switch click to switch between modes
// initliize all states of lamp
// generate target positions for each servo from either joysick, pot, or intitial state of mode
// prepare data to be transmitted
// transmite data over radio


void loop() 
{
  if(checkTime(startTime1, duration1) == true) 
  {
    startTime1 = millis();
    ledWhite();
    delay(10);
  }

  joySW = checkJoySW();                             // read joystick button
  initializing = initializeState(initializing);

  if(armState == true)
  { 
    changeArmState = false;
    mode = checkMode(joySW, joySWLast);
    joySWLast = joySW;

    if(changeArmState == true && initializing == false)
    {
      armState = false;
      changeArmState = false;
    }


    if(mode = 0)  //spotlight
    {
      lampBrightness = 255;
      mirrorState = false;

      if(initializing == true)
      {
        targetX = degMid;
        targetY = degMid;
      }

      if(initializing == false)
      {
        joyX = -readJoyX();
        joyY = -readJoyY();
      }

    }

    if(mode = 1)
    {
       lampBrightness = 255;
      mirrorState = true;

      if(initializing == true)
      {
        targetX = 40;
        targetY = 120;
      }

      if(initializing == false)
      {
        joyX = -readJoyX();
        joyY = -readJoyY();
      }
    }


    if(checkArmState(joySW) == 0)
    {
      targetX = degMin;
      targetY = degMax;
      lampBrightness = 0;
      mirrorState = false;
      initializing = true;
      changeArmState = true;  // we are shutting down
      ledLightBlue();
    }
     
  }

  if(armState == false)
  {
    armState = checkArmState(joySW); 
  }




  // -------------------- initialize position ---------------------- //
  // If new state is selected
  initializing = initializeState(initializing);

  switch(armState)
  {
    default:    // offline
      targetX = degMin;
      targetY = degMax;
      lampBrightness = 0;
      mirrorState = false;
      initializing = true;
      mode = 0;
      ledLightBlue();
    break;

    case 1: // online
      mode = checkMode(joySW, joySWLast); 
      joySWLast = joySW;

      switch(mode)
      { 
        default: // spotlight
          targetX = degMid;
          targetY = degMid;
          lampBrightness = 255;
          mirrorState = false;
        break;

        case 1: // mirror
          targetX = 40;
          targetY = 120;
          lampBrightness = 255;
          mirrorState = true;
        break;

        case 2: // auto
          targetX = degMid;
          targetY = degMid;
          lampBrightness = 255;
          mirrorState = false;
        break;
      }
    break;
  }

  if(initializing == false && armState == true)
  {
    // -------------------- Joysitck Modes ---------------------- //
    if(mode == 0 || mode == 1)
    {
      if(mode == 0) {ledRed();}     // Spotlight
      if(mode == 1) {ledGreen();}  // mirror

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
      lampPos = (wheelRot/wheelRotMid) * 90.0;      // [-90:90] lamp position from forward 
      targetX = lampPos + degMid;                  // abosolute positioning from 90
      degs[0] = moveServo(targetX, degs[0]);
      degs[1] = moveServo(targetY, degs[1]);
      ledBlue();
    }
  } // end Online
   
    
  dataPrep();
  dataTransmit();
  print1();
  delay(delay1);
}
