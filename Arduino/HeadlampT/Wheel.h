#ifndef Wheel_h
#define Wheel_h


#define potPin A3

float potVolt;
const int potVin = 5;     
const float potVMin = .12;
const float potVMax = 4.94;

float potRot;    // rotations of pot
const int potRotMax = 10;     
const int potRotMid = 5;

float wheelRot;
float wheelCal;                         // inital position of wheel
const float wheelRotMax = 5;            // complete wheel rotations
const float wheelRotMid = wheelRotMax/2;  // wheeel rotations from center

int potVal;            // digital output of steering pot

float lampPos; // relative lamp position from forward
float lampPosSensitivity = 1.25;   // how sensative is the lamp. instea of 1:1 ratio with wheel its 1:1.25 ratio


float readWheelRot()
{
  potVal = analogRead(potPin);                                            // read pot input [0:1023]
  potVolt = voltRead(potPin, potVin);                                     // reads [potVMin:potVMax] due to voltage losses
  potVolt = linearMap(potVolt, potVMin, potVMax, 0, potVin);              // normalize to [0:5]
  potRot = linearMap(potVolt, 0, potVin, 0, potRotMax) - potRotMid;       // pot rotation [-5:5]
  wheelRot = -1 * potRot;                                                 // wheel rotation
  return wheelRot;                                                        // maxes out at [-2.5:2.5] due to steering wheel
}

void wheelSetup()
{
  pinMode(potPin, INPUT);
  wheelCal = readWheelRot(); // set wheel at neutral to initiate
}


#endif
