#ifndef Wheel_h
#define Wheel_h


#define potPin A3

const int potMin = 0;
const int potMax = 800;
const int potMid = 400;
const int potVin = 5;        //?????
const int potRotMax = 10;     
const int potRotMid = 5;
const int potDegMax = potRotMax * 360;  // 3600

const float wheelRotMax = 2.5;
const int wheelPotMid = 0;            //// remove


float wheelCal;            // inital position of wheel
float wheelRot;

int potVal;            // position of steering pot
int potDeg;
float potVolt;
float potRot;    // rotations of pot

float lampPos; // relative lamp position from forward





float readWheelRot()
{
  potVolt = voltRead(potPin, potVin);                                            // read pot voltage [0:5]
  potRot = linearMap(potVolt, 0, potVin, 0, potRotMax) - potRotMid;          // pot rotation [-5:5]
  wheelRot = -1 * potRot;                                                           // wheel rotation [-2.5;2.5]
  return wheelRot;   
}

void wheelSetup()
{
  pinMode(potPin, INPUT);
  wheelCal = readWheelRot(); // set wheel at neutral to initiate
}


#endif
