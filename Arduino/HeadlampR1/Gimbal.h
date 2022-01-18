#ifndef Gimbal_h
#define Gimbal_h

// ------------ 270 servo x left right ------------- //
#include <Servo.h>
const int xServoPin = 3;
Servo servoX;                                                // servo instance
int xInput;                                              // recieved servo position data

const int xBoundMin = 6;                                    // minimum start position 
const int xBoundRange = 180;                                // range
const int xBoundMax = xBoundMin + xBoundRange;          // maximum end position
const float xLimitMin = 0.0;                           // mechanical min limit of possible positions
const float xLimitMax = 241.0;                        // mechanical max limit, adjusting here sets the spacing ratio correctly
float xCal;                                              // calibrated x servo position                   

// ms conversion
const float xMsMax = 2400.0;                                              // mechanical ms max
const float xMsMin = 544.0;                                               // mechanical ms min
const float xMsPerDeg = ((xMsMax - xMsMin)/(xLimitMax - xLimitMin));     // ratio between ms to degrees
const float xMsB = xMsMin - (xMsPerDeg * xLimitMin);                       // y intersept
float xMs;                                                                 // ms variable


// ----------- 180 servo y up down ------------- //
const int yServoPin = 2;
Servo servoY;                                    // servo instance
int yInput;                                      // recieved servo position data

const int yInputMin = 0;                        // min input 
const int yInputMax = 180;                      // max input 

const int yBoundMin = 0;                        // minimum digital position
const int yBoundMax = 156;                      // maximum digital position verified
const int yLimitMax = 174;                      // maximum mechanical position before failure
const int yLimitMin = 0;                        // minimum mechanical position before failure
const int yShift = yBoundMax - yInputMax;       // shifting the angle plane
float yCal;                                      // calibrated servo position       

void gimbalSetup()
{
    servoX.attach(xServoPin);
    servoY.attach(yServoPin);
}          


#endif
