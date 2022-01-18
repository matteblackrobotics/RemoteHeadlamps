// port 1410

// Include Libraries
#include "Arduino.h"
#include "Radio.h"
#include "PrintOut.h"
#include "Utilities.h"
#include "Gimbal.h"

#include <MBR_print.h>

// ----- switch ---- //
bool joySW;

// -------------- Lamp ------------- //
const int lampPin = 5;   // mosfet
int lampBrightness = 0;

// ------------------ VOID SETUP -------------- //
void setup() 
{ 
  utilitiesSetup();
  gimbalSetup();
  radioSetup(); 
  pinMode(lampPin, OUTPUT); 
}


// ------------------- VOID LOOP ------------------ //
void loop() 
{
  myRadio.startListening();
  if (myRadio.available())
  {
    while(myRadio.available())
    {
      myRadio.read(&dataT, sizeof(dataT));
      
      lampBrightness = dataT.lampBrightness;
      analogWrite(lampPin, lampBrightness);                  // lamp brightness

      joySW = dataT.joySW;

      // 180 servo y = degrees
      yInput = dataT.servoY;                                // assign data                      
      yCal = yInput + yShift;                               // shifted plane
      yCal = f_bound(yCal, yBoundMin, yBoundMax);           // bounds  

      // 270 servo x
      xInput = dataT.servoX;                                 // assign data
      xCal = xInput + xBoundMin;                            // start 
      xCal = f_bound(xCal, xBoundMin, xBoundMax);         // bound
      xMs = (xMsPerDeg * xCal) + xMsB;                   // convert degress to microseond pulses
      
      // write servos
      servoX.writeMicroseconds(xMs);                          // Left Right 270 servo calibrated value
      servoY.write(yCal);                                     // normal write to motor calibrated value

      print1();
      
    }
  }
  else
  {
    // Serial.println("No Radio Available, Reboting");      
  }
}

// ------------- Bound ---------------- //
float f_bound(float x, int xMin, int xMax)
{
  if(x > xMax) {x = xMax;}
  if(x < xMin) {x = xMin;}
  return x;
}


// Degrees to Ms
float f_DegToMs(int limit_min, int limit_max, int deg)
{
  float ms_min = 544.0;                                           // min ms pulse
  float ms_max = 2400.0;                                          // max ms pulse
  float ms_per_deg = (ms_max - ms_min)/(limit_max - limit_min);   // slope
  float ms_b = ms_min - (ms_per_deg * limit_min);                 // y intercept
  float ms = (ms_per_deg * deg) + ms_b;                           // conversion
  return ms;
}

// specific to motor x
float f_ms_x(int deg)
{
  float ms = f_DegToMs(xLimitMin, xLimitMax, deg);
  return ms;
}

// specific to motor y
float f_ms_y(int deg)
{
  float ms = f_DegToMs(yLimitMin, yLimitMax, deg);
  return ms;
}

void print1()
{
  printString(FileName);
  printBool("joySW", joySW);
  printInt("brightness", lampBrightness);
  printFloat("xInput", xInput);
  printFloat("xCal", xCal);
  printFloat("xMS", xMs);
  printFloat("yInput", yInput);
  printFloat("yCal", yCal);
  printLn();
}
