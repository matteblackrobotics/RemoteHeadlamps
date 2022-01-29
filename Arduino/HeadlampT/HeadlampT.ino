// 

// /dev/cu.usbserial-14320
// explore public and private variabales
// explore functions

// make timers for button presses and transmission

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
    // disarmed
    case 0:
      lampBrightness = 0;
      mirrorState = false;
      mode = 2;
      targetX = degMin;
      targetY = degMax;
      switch(initializing)
      {
        case 1: // initializing
          ledYellow;
          initializing = checkInitializing();
        break;    

        case 0: // complete
          ledLightBlue();
        break;
      }
    break;
    
    // armed
    case 1: 
      mode = checkMode();
      joySWLast = joySW;
      switch(mode)
      {
        case 0:  // spotlight mode
          mirrorState = false;
          switch(initializing)
          {
            case 1: // initialiazing
              ledYellow();
              targetX = degMid;
              targetY = degMid;
              initializing = checkInitializing();
            break;

            case 0: // readjoy
              lampBrightness = 100;
              ledRed();
              joyToTarget();
            break;
          }
        break;

        case 1: // mirror mode
          mirrorState = true;
          switch(initializing)
          {
            case 1: // initializing
              ledYellow();
              targetX = 40;
              targetY = 120;
              initializing = checkInitializing();
            break;

            case 0: // readJoy
              lampBrightness = 100;
              ledGreen();
              joyToTarget();
            break;
          }
        break;

        case 2: // auto mode
          mirrorState = false;
          switch(initializing)
          {
            case 1: // initializing
              targetX = degMid;
              targetY = degMid;
              initializing = checkInitializing();
            break;

            case 0: // read wheelPot
              lampBrightness = 100;
              ledBlue();
              wheelRot = readWheelRot() - wheelCal;           // wheel position with calibration 
              lampPos = -(wheelRot/wheelRotMid) * 90.0;       // [-90:90] lamp position from forward 
              lampPos = lampPos * lampPosSensitivity;         // multiplier
              targetX = lampPos + degMid;                     // abosolute positioning from 90
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
  
  delay(10);

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
