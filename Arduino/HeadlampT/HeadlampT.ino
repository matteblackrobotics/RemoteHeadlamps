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

  joySW = checkJoySW();                       // read joystick button
  armState = checkArmState(joySW);           // long button press?

  switch(armState)
  {
    case 0:
      lampBrightness = 0;
      mirrorState = false;
      mode = 2;
      targetX = degMin;
      targetY = degMax;
      switch(initializing)
      {
        case 1:
          ledYellow;
          initializing = checkInitializing();
        break;    

        case 0:
          ledLightBlue();
        break;
      }
    break;

    case 1: 
      mode = checkMode();
      joySWLast = joySW;
      switch(mode)
      {
        case 0:  // spotlight
          lampBrightness = 255;
          mirrorState = false;
          switch(initializing)
          {
            case 1: 
              ledYellow();
              targetX = degMid;
              targetY = degMid;
              initializing = checkInitializing();
            break;

            case 0:
              ledRed();
              joyToTarget();
            break;
          }
        break;

        case 1: // mirror
          lampBrightness = 255;
          mirrorState = true;
          switch(initializing)
          {
            case 1:
              ledYellow();
              targetX = 40;
              targetY = 120;
              initializing = checkInitializing();
            break;

            case 0:
              ledGreen();
              joyToTarget();
            break;
          }
        break;

        case 2: // auto
          lampBrightness = 255;
          mirrorState = false;
          switch(initializing)
          {
            case 1:
              targetX = degMid;
              targetY = degMid;
              initializing = checkInitializing();
            break;

            case 0:
              ledBlue();
              wheelRot = readWheelRot() - wheelCal;         // wheel position with calibration 
              lampPos = (wheelRot/wheelRotMid) * 90.0;      // [-90:90] lamp position from forward 
              targetX = lampPos + degMid;                  // abosolute positioning from 90
              targetY = degMid;
            break;
          }
        break;
      }
    break;
  } // end switch cases

  degs[0] = servoDeg(targetX, degs[0]);
  degs[1] = servoDeg(targetY, degs[1]);
  dataPrep();
  dataTransmit();
  
  printJoystick();
  printGimbal();
  // printWheel();
  printLn();
  
  delay(0);

} // end void loop


void joyToTarget()
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
}
