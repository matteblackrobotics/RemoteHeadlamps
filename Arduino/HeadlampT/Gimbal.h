#ifndef Gimbal_h
#define Gimbal_h


// ------------------------------------------- Servos ------------------------------------------ //
int servoXdeg;                // individual servo position
int servoYdeg; 

int degMax = 180;    // min bound for servo position due to power issues??    // desired max
int degMin = 0;    // min bound for servo position due to power issues??     // desired min
int degMid = 180/2;  // correct mid position for servo

int degs[4] = {degMin, degMax, degMin, degMax};      // servo degree array {x1, y1, x2, y2} initial position
int sizeDegs = sizeof(degs)/sizeof(degs[0]);           // size of degree array
int mech0s[4] = {0, 0, 0, 0};                           // mech 0 (deg)
int steps[2] = {0, 0};                                   // step x, y

int targetX; 
int targetY;
int stepX; 
int stepY;
int stepMax = 5;
int stepMin = 1; 
int stepThresh1 = 10;


int moveServo(int targetDeg, int deg)
{
  if(targetDeg > deg)                          // move positive
  {
    if(targetDeg - deg >= 10) {deg = deg + stepMax;}  // big step
    else{deg = deg + stepMin;}                          // small step
  }

  if(targetDeg < deg)                          // move negative
  {
    if(deg - targetDeg >= 10) {deg = deg - stepMax;}  // big step
    else{deg = deg - stepMin;}                            // small step
  }
  return deg;                                   // updated servo degrees
}


#endif
